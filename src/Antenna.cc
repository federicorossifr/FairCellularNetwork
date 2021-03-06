#include "Antenna.h"
Define_Module(Antenna);

Antenna::Antenna() {
    //Initialize Antenna Frame
    frame = new Frame();
    for(int i=0; i<25; i++)
        frame->set_rbs(i,new ResourceBlock());
}

/* This method is used to create statistics with
 * runtime-choose name, for user queues.
 */
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
    emptyRB = registerSignal("emptyRB");

    //Retrieve network dimensions
    int dim = (int)par("n");
    networkDimension = dim;

    //Initialize User Descriptors
    for(int i = 0; i < dim; ++i) {
        UserDescriptor* tmp = new UserDescriptor();
        tmp->setID(i);
        users.push_back(tmp);
        queueSizeSignals.push_back(createUserQueueSizeSignal(i));
        cMessage* timTmp = new cMessage((std::to_string(i)).c_str());
        packetTimers.push_back(timTmp);
    }
    timeSlotTimer->setSchedulingPriority(9999);

    //Schedule a timer for the Timeslot
    useWindow = par("windowing");
    windowWeight = par("windowingWeight");
    period = par("timeSlotPeriod");
    scheduleAt(simTime()+ period,timeSlotTimer);
    packetMeanIntTime = par("packetMeanIntTime");

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
    std::sort(tmpUsers.begin(),tmpUsers.end(),Antenna::compareUsers);
    int availableResourceBlocks = 25;
    int currResourceBlockIndex = 0;
	double bytesSent=0;
	int packetSent=0;
	int queueCount=0;

	//Empty frame and resource blocks to be used in this timeslot
    resetFrame();
    for(auto user:tmpUsers) {
        emit(queueSizeSignals.at(user->getID()),user->packetCount());
        if(availableResourceBlocks < 1 || currResourceBlockIndex >= 25) {
            emit(throughputSignal,0);
            continue; //We do not break here to emit throughput "0" for each user
        }
        int totalBytePacked = 0;
        int cqi = user->getCQI();
        EV << "CQI: " << cqi << endl;
        //CQI maybe not available, skip this user if so
        if(cqi < 1) continue;
        //Retrieve Resource Block size for the current user
        int rbSize = cqiMap[cqi-1];
        //Initialize first resource block;
        ResourceBlock* rb = frame->get_rbs(currResourceBlockIndex);
        rb->setSize(rbSize);
        rb->setUserID(user->getID());
        Packet* tmp;
        while( user->hasPacket() ) {
            tmp = user->popPacket();
			bytesSent+=tmp->getSize();
            int spaceAfter = rb->insertPacket(tmp);

            //Go to next Resource Block if current is full
            if(spaceAfter==0) {
                availableResourceBlocks--;
                if(availableResourceBlocks > 0) {
                    rb=frame->get_rbs(++currResourceBlockIndex);
                    rb->setSize(rbSize);
                    rb->setUserID(user->getID());
                }
            }
            //Whole packet successfully inserted in the current resource block
            if(spaceAfter>=0) {
                totalBytePacked+=tmp->getSize();
                packetSent++;
                emit(packetSentSignal,1);
                emit(queueingTimeSignal,simTime()-tmp->getCreation());
                continue;
            }
            //Here packet could not be inserted in current RB because it's too big
            //Check if there is available space to insert it fragmented
            //Size available is sum of the portion of current RB and the rest of resources block
            if(tmp->getSize() <= rb->getAvailable()+(availableResourceBlocks-1)*rbSize) {
                //Mark the packet as a "fragment"
                tmp->setFragment(true);
                Packet* tmpDup = tmp;

                //Insert and fragment until packed size is equal to packet size
                while(tmpDup->getSize() > tmpDup->getPackedSize()) {

                    //If ResourceBlock is full go to next ResourceBlock
                    if(rb->insertPacket(tmpDup) <= 0) {
                        availableResourceBlocks--;
                        if(availableResourceBlocks > 0) {
                            rb=frame->get_rbs(++currResourceBlockIndex);
                            rb->setSize(rbSize);
                            rb->setUserID(user->getID());
                        }
                    }
                    //If the packet has not been inserted create another copy as a fragment
                    //This is required by Omnet++ to have an unique holder for each packet.
                    if(tmpDup->getSize() > tmpDup->getPackedSize()) tmpDup = tmpDup->dup();
                }
                totalBytePacked+=tmp->getSize();
                packetSent++;
                emit(packetSentSignal,1);
                emit(queueingTimeSignal,simTime()-tmp->getCreation());
            } else {
                //Undo previous operation done before
                //knowing if packet could be inserted
				bytesSent-=tmp->getSize();
                user->undoPopPacket(tmp);

                //We'll skip to the next user if the packet cannot be inserted in this turn
                break;
            }
        }

        //Use or not the weighted historical average
        //according to the parameter in the Antenna.ned file.
        if(useWindow)
            user->setRCVBT(totalBytePacked,windowWeight);
        else
            user->setRCVBT(totalBytePacked);

        //Emit throughput for the current user
        //Note that the same value could be obtained
        //summing up all the "totalBytePacked" and
        //then dividing by simualation duration,
        //since all timeslots last the same amount
        //of time (so no ratio game here)
        emit(throughputSignal,totalBytePacked/period);
        //If ResourceBlock is not empty go to next
        if(!rb->isEmpty()) {
            currResourceBlockIndex++;
            availableResourceBlocks--;
        }
    }

    //Emit queue size statistic by
    //summing up all the packet in different
    //user queues.
    for(auto user:tmpUsers)
            queueCount+=user->packetCount();
    for (int i = 0; i < networkDimension; ++i)
        send(frame->dup(),"out",i);
    emit(emptyRB,availableResourceBlocks);
	emit(packetPerSlotSignal,packetSent);
	emit(queuedPacketsSignal,queueCount);
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
