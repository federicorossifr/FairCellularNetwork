#include "Antenna.h"

Define_Module(Antenna);

void Antenna::initialize()
{
    scheduleAt(simTime()+"timeSlotPeriod",timeSlotTimer);
}

void Antenna::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        // TODO -- Handle self timer
        scheduleAt(simTime()+"timeSlotPeriod",timeSlotTimer);
    }
}
