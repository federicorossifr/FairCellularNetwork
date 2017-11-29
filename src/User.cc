#include "User.h"

Define_Module(User);

void User::initialize()
{
    scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
}

void User::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        // TODO - Send CQI special message
        scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
    }
}
