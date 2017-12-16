#include "Antenna.h"
Define_Module(Antenna);

Antenna::Antenna() {
    //Initialize Antenna Frame
    frame = new Frame();
    for(int i=0; i<25; i++)
        frame->set_rbs(i,new ResourceBlock());
}

simsignal_t Antenna::createUserQueueSizeSignal(int userId) {
    char signalName[32];
    sprintf(signalName,"user%dqueueSize", userId);
    simsignal_t sig = registerSignal(signalName);
    char statisticName[32];
    sprintf(statisticName,"user%dqueueSize", userId);
    cProperty* statisticTemplate = getProperties()->get("statisticTemplate","queueSize");
    getEnvir()->addResultRecorders(this, sig, statisticName, statisticTemplate);
    return sig;
}

void Antenna::initialize()
{
    throughputSignal = registerSignal("throughput");
    packetPerSlotSignal = registerSignal("packetsPerSlot");
    packetSentSignal = registerSignal("packetsSent");
    queueingTimeSignal = registerSignal("queueingTime");
    queuedPacketsSignal = registerSignal("queuedPacketsPerSlot");
    //Retrieve network dimensions
    int dim = (int)par("n");
    networkDimension = dim;

    //Initialize User Descriptors
    for(int i = 0; i < dim; ++i) {
        UserDescriptor* tmp = new UserDescriptor();
        tmp->setID(i);        users.push_back(tmp);
        queueSizeSignals.push_back(createUserQueueSizeSignal(i));
        cMessage* timTmp = new cMessage((std::to_string(i)).c_str());
        packetTimers.push_back(timTmp);
    }
    timeSlotTimer->setSchedulingPriority(9999);

    //Schedule a timer for the Timeslot
    period = par("timeSlotPeriod");
    EV << period << endl;
    scheduleAt(simTime()+ period,timeSlotTimer);
    packetMeanIntTime = par("packetMeanIntTime");
    timeslotWindow = par("timeslotWindow");
    //Schedule a timer for each queue
    for(auto tim:packetTimers) {
        scheduleAt(simTime()+exponential(packetMeanIntTime,par("exponentialIntArrRNGID")),tim);
    }
}

void Antenna::handleCQIMessage(Cqi* cqiMsg) {
    int uid = cqiMsg->getUserID();
    int val = cqiMsg->getCqiValue();
    EV << "CQI Message from -- " << uid << " -- CQI value -- " << val << endl;
    if(uid < networkDimension)
        (users.at(uid))->setCQI(val);
    delete(cqiMsg);
}

void Antenna::handleExpInterrarival(cMessage* msg) {
    int index = std::stoul(msg->getName());
    Packet* pkt = new Packet();
    pkt->setSize((int)uniform(1,75,par("exponentialSizeRNGID")));
    pkt->setCreation(simTime());
    (users.at(index))->insertPacket(pkt);
    int queueCount = 0;
    for(auto user: users)
        queueCount+=user->packetCount();
    emit(queuedPacketsSignal,queueCount);
    scheduleAt(simTime()+exponential(packetMeanIntTime,par("exponentialIntArrRNGID")),packetTimers.at(index));
}

void Antenna::handleTimeSlot() {
    //Sorting user descriptors using receivedBytes (sorting out of place)
    std::vector<UserDescriptor*> tmpUsers = users;
    if(timeslotWindow == elapsedTimeslots) {
        for(auto user:users) {
            user->resetRCVBT();
        }
        EV << "rESET" << endl;
        elapsedTimeslots=0;
    }
    elapsedTimeslots++;
    std::sort(tmpUsers.begin(),tmpUsers.end(),Antenna::compareUsers);
    int availableResourceBlocks = 25;
    int currResourceBlockIndex = 0;
	double bytesSent=0;
	int packetSent=0;
	int queueCount=0;
    resetFrame();
    for(auto user:tmpUsers) {
        emit(queueSizeSignals.at(user->getID()),user->packetCount());
        if(availableResourceBlocks < 1 || currResourceBlockIndex >= 25) {
            emit(throughputSignal,0);
            continue; //Needed to emit queue size for all users
        }
        int totalBytePacked = 0;
        EV << "==========================================================" << endl;
        EV << "Serving user -- " << user->getID() << endl;
        EV << "Number of packet in queue for user -- " << user->packetCount() << endl;

        EV << "Total bytes received so far -- " << user->getRCVBT() << endl;
        EV << "Available resource blocks -- " << availableResourceBlocks << endl;
        //Retrieve CQI for the user
        int cqi = user->getCQI();
        EV << "CQI: " << cqi << endl;
        //First turn, cqi not available
        if(cqi < 1) continue;

        //Retrieve Resource Block size for the current user
        int rbSize = cqiMap[cqi-1];
        EV << "ResourceBlock size: " << rbSize << endl;
        //EV << "-------------------" << endl;

        //Initialize first resource block;
        ResourceBlock* rb = frame->get_rbs(currResourceBlockIndex);
        rb->setSize(rbSize);
        rb->setUserID(user->getID());
        Packet* tmp;
        while( user->hasPacket() ) {
            tmp = user->popPacket();
			bytesSent+=tmp->getSize();
            //EV << "Processing packet -- " << tmp->getId() << endl;
            //EV << "Packet size -- " << tmp->getSize() << endl;
            //EV << "Current ResourceBlock -- " << currResourceBlockIndex << endl;
            //Try to insert packet in ResourceBlock
            int spaceAfter = rb->insertPacket(tmp);

            //Go to next Resource Block if current is full
            if(spaceAfter==0) {
                //EV << "Resource block -- " << currResourceBlockIndex << "full" << endl;
                availableResourceBlocks--;
                if(availableResourceBlocks > 0) {
                    rb=frame->get_rbs(++currResourceBlockIndex);
                    rb->setSize(rbSize);
                    rb->setUserID(user->getID());
                }
            }
            if(spaceAfter>=0) {
                //EV << "Packet -- " << tmp->getId() << " inserted in ResourceBlock -- " << currResourceBlockIndex-((spaceAfter)?0:1) << endl;
                totalBytePacked+=tmp->getSize();
                packetSent++;
                emit(packetSentSignal,1);
                emit(queueingTimeSignal,simTime()-tmp->getCreation());
                //EV << "-------------------" << endl;
                continue;
            }

            //Here packet could not be inserted in current RB because it's too big
            //Check if there is available space to insert it fragmented
            //Size available is sum of the portion of current RB and the rest of resources block
            //EV << "Total available space: " << (rb->getAvailable()+(availableResourceBlocks-1)*rbSize) << endl;
            if(tmp->getSize() <= rb->getAvailable()+(availableResourceBlocks-1)*rbSize) {
                //Mark tmp as fragmented
                //EV << "Packet -- " << tmp->getId() << " can be fragmented" << endl;
                tmp->setFragment(true);
                Packet* tmpDup = tmp;
                //Insert and fragment until packed size is equal to packet size
                while(tmpDup->getSize() > tmpDup->getPackedSize()) {
                    //If ResourceBlock is full go to next ResourceBlock;
                    if(rb->insertPacket(tmpDup) <= 0) {
                        availableResourceBlocks--;
                        if(availableResourceBlocks > 0) {
                            rb=frame->get_rbs(++currResourceBlockIndex);
                            rb->setSize(rbSize);
                            rb->setUserID(user->getID());
                        }
                    }
                    //EV << "Packet -- " << tmpDup->getTreeId() << " fragment inserted" << endl;
                    //EV << "Total packed size so far: " << tmpDup->getPackedSize() << endl;
                    if(tmpDup->getSize() > tmpDup->getPackedSize()) tmpDup = tmpDup->dup();
                }
                totalBytePacked+=tmp->getSize();
                packetSent++;
                emit(packetSentSignal,1);
                emit(queueingTimeSignal,simTime()-tmp->getCreation());
            } else {
				bytesSent-=tmp->getSize();
                user->undoPopPacket(tmp);
                // TODO - What to do??
                break;
            }
            //EV << "-------------------" << endl;
        }
        EV << "User -- " << user->getID() << " served with -- " << totalBytePacked << " bytes" << endl;
        //EV << "==========================================================" << endl;
        user->setRCVBT(totalBytePacked);
        emit(throughputSignal,totalBytePacked/period);
        //If ResourceBlock is not empty go to next
        if(!rb->isEmpty()) {
            //EV << "ResourceBlock -- " << currResourceBlockIndex << " is not empty, skipping it" << endl;
            currResourceBlockIndex++;
            availableResourceBlocks--;
        }
    }
    for(auto user:tmpUsers)
            queueCount+=user->packetCount();
    for (int i = 0; i < networkDimension; ++i)
        send(frame->dup(),"out",i);
	//EV << "Bytes sent in this timeslot: " << bytesSent;
	emit(packetPerSlotSignal,packetSent);
	emit(queuedPacketsSignal,queueCount);
	// TODO - Emit queue packet count here
}

void Antenna::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        if(msg->isName("antennaTimeSlot")) {
            EV << "Timeslot" << endl;
            handleTimeSlot();
            scheduleAt(simTime()+period,timeSlotTimer);
        } else {
            handleExpInterrarival(msg);
        }
    } else if(msg->isName("CQI")) { //Handle CQI message from users
        handleCQIMessage((Cqi*)msg);
    }
}

void Antenna::finish() {
    //Cleanup timeSlotTimer
    cancelAndDelete(timeSlotTimer);

    //Cleaning up timers
    for(auto tim:packetTimers) cancelAndDelete(tim);
    for(auto user:users) delete(user);
    //Clean up frame and packet queues in frame
    for(int i = 0; i < 25; ++i)
        delete(frame->get_rbs(i));
    delete(frame);
}
