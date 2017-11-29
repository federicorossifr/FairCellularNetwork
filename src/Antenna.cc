#include "Antenna.h"
Define_Module(Antenna);

void Antenna::initialize()
{
    //Retrieve network dimensions
    int dim = (int)par("n");
    networkDimension = dim;
     //Create an array of users descriptors
    users = new UserDescriptor[dim];
    //Create array of Timers for user packets
    packetTimers = new cMessage[dim];
    //Initialize User Descriptors
    for(int i = 0; i < dim; ++i) {        users[i].setID(i);
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
    users[uid].setCQI(val);
    delete(cqiMsg);
}

void Antenna::handleExpInterrarival(cMessage* msg) {
    int index = std::stoul(msg->getName());
    Packet* pkt = new Packet();
    pkt->setSize((int)uniform(1,75));
    pkt->setCreation(simTime());
    users[index].insertPacket(pkt);
    scheduleAt(simTime()+exponential(1),(cMessage*)&packetTimers[index]);
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
