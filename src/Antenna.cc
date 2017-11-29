#include "Antenna.h"

Define_Module(Antenna);

void Antenna::initialize()
{
    scheduleAt(simTime()+ par("timeSlotPeriod"),timeSlotTimer);
}

void Antenna::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        scheduleAt(simTime()+ par("timeSlotPeriod"),timeSlotTimer);
    }
}
