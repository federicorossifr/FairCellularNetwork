#ifndef __FAIRCELLULARNETWORK_USER_H_
#define __FAIRCELLULARNETWORK_USER_H_

#include <omnetpp.h>

using namespace omnetpp;


class User : public cSimpleModule
{
    cMessage* timeSlotTimer = new cMessage("timeSlot");
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
