#include "Antenna.h"
Define_Module(Antenna);

void Antenna::initialize()
{
    timeSlotTimer->setSchedulingPriority(9999);
    scheduleAt(simTime()+ par("timeSlotPeriod"),timeSlotTimer);
    for(int i = 0; i < networkDimension; ++i) {
        scheduleAt(simTime()+exponential(5),&packetTimers[i]);
    }
}

void Antenna::handleCQIMessage(Cqi* cqiMsg) {
    int uid = cqiMsg->getUserID();
    int val = cqiMsg->getCqiValue();
    users[uid].setCQI(val);
    delete(cqiMsg);
}

void Antenna::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        scheduleAt(simTime()+ par("timeSlotPeriod"),timeSlotTimer);
    } else if(msg->isName("CQI")) { //Handle CQI message from users
        handleCQIMessage((Cqi*)msg);
    }
}
