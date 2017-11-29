#include "Antenna.h"
Define_Module(Antenna);

Antenna::Antenna() {
    //Initialize Antenna Frame
    frame = new Frame();
    for(int i=0; i<25; i++)
        frame->set_rbs(i,new ResourceBlock());
}

void Antenna::initialize()
{
    //Retrieve network dimensions
    int dim = (int)par("n");
    networkDimension = dim;

    //Create array of Timers for user packets
    packetTimers = new cMessage[dim];
    //Initialize User Descriptors
    for(int i = 0; i < dim; ++i) {
        UserDescriptor* tmp = new UserDescriptor();
        tmp->setID(i);        users.push_back(tmp);
        packetTimers[i].setName( (std::to_string(i)).c_str() );
    }
    timeSlotTimer->setSchedulingPriority(9999);
    period = par("timeSlotPeriod");
    scheduleAt(simTime()+ period,timeSlotTimer);
    EV << period << endl;
    for(int i = 0; i < networkDimension; ++i) {
        scheduleAt(simTime()+exponential(1),(cMessage*)&packetTimers[i]);
    }
}

void Antenna::handleCQIMessage(Cqi* cqiMsg) {
    int uid = cqiMsg->getUserID();
    int val = cqiMsg->getCqiValue();
    (users.at(uid))->setCQI(val);
    delete(cqiMsg);
}

void Antenna::handleExpInterrarival(cMessage* msg) {
    int index = std::stoul(msg->getName());
    Packet* pkt = new Packet();
    pkt->setSize((int)uniform(1,75));
    pkt->setCreation(simTime());
    (users.at(index))->insertPacket(pkt);
    scheduleAt(simTime()+exponential(1),(cMessage*)&packetTimers[index]);
}

void Antenna::handleTimeSlot() {
    //Sorting user descriptors using receivedBytes
    std::sort(users.begin(),users.end(),Antenna::compareUsers);
    int availableResourceBlocks = 25;
    int currResourceBlockIndex = 0;
    for(auto user:users) {
        if(availableResourceBlocks < 1) break;
        //Retrieve CQI for the user
        int cqi = user->getCQI();

        //First turn, cqi not available
        if(cqi < 1) continue;

        //Retrieve Resource Block size for the current user
        int rbSize = cqiMap[cqi-1];
        ResourceBlock* rb = frame->get_rbs(currResourceBlockIndex);
        rb->setSize(rbSize);
        rb->setUserID(user->getID());
        Packet* tmp;
        while( (tmp = user->getHeadPacket()) ) {



        }
        int spaceAfter = rb->insertPacket()
    }


    // TODO - Broadcast the frame
}

void Antenna::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        if(msg->isName("antennaTimeSlot")) {
            EV << "Timeslot" << endl;
            scheduleAt(simTime()+period,timeSlotTimer);
        } else {
            handleExpInterrarival(msg);
        }
    } else if(msg->isName("CQI")) { //Handle CQI message from users
        handleCQIMessage((Cqi*)msg);
    }
}
