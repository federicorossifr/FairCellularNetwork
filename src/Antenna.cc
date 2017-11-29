#include "Antenna.h"

Define_Module(Antenna);

void Antenna::initialize()
{
    timeSlotTimer->setSchedulingPriority(9999);
    scheduleAt(simTime()+ par("timeSlotPeriod"),timeSlotTimer);
}

void Antenna::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        scheduleAt(simTime()+ par("timeSlotPeriod"),timeSlotTimer);
    }
}
