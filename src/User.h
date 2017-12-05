#ifndef __FAIRCELLULARNETWORK_USER_H_
#define __FAIRCELLULARNETWORK_USER_H_

#include <omnetpp.h>
#include "Cqi_m.h"
#include "Packet_m.h"
#include "Frame_m.h"
using namespace omnetpp;


class User : public cSimpleModule
{
    simsignal_t resp_signal,packet_signal;
    int userID;
    cMessage* timeSlotTimer = new cMessage("timeSlot");
    int cqiSum=0,cqiCount=0;
    int computeCqi();
    static int USER_COUNTER;
    void handleFrame(Frame*);
  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
  public:
    User() {userID=USER_COUNTER++;}
};

#endif
