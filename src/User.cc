#include "User.h"
int User::USER_COUNTER=0;
Define_Module(User);

void User::initialize()
{
    scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
}

int User::computeCqi() {
    int cqi = 1;
    // TODO - Genrate a cqi number
    return cqiMap[cqi-1];
}

void User::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        // TODO - Send CQI special message
        scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
    }
}
