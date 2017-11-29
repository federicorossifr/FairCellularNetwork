#ifndef __FAIRCELLULARNETWORK_USER_H_
#define __FAIRCELLULARNETWORK_USER_H_

#include <omnetpp.h>
#include "Cqi_m.h"
#include "Packet_m.h"
#include "Frame_m.h"
using namespace omnetpp;


class User : public cSimpleModule
{
    int userID;
    cMessage* timeSlotTimer = new cMessage("timeSlot");
    int cqiMap[15] = {3,3,6,11,15,20,25,36,39,50,63,72,80,93,93};
    int computeCqi();
    static int USER_COUNTER;
    void handleFrame(Frame*);
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
  public:
    User() {userID=USER_COUNTER++;}
};

#endif
