#ifndef __FAIRCELLULARNETWORK_ANTENNA_H_
#define __FAIRCELLULARNETWORK_ANTENNA_H_

#include <omnetpp.h>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Antenna : public cSimpleModule
{
    cMessage* timeSlotTimer = new cMessage("timeSlot");
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
