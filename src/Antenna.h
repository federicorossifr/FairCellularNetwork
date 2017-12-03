#ifndef __FAIRCELLULARNETWORK_ANTENNA_H_
#define __FAIRCELLULARNETWORK_ANTENNA_H_

#include <omnetpp.h>
#include <string>
#include <algorithm>
#include <vector>
#include "UserDescriptor.h"
#include "Cqi_m.h"
#include "Frame_m.h"
using namespace omnetpp;

class Antenna : public cSimpleModule
{
    cMessage* timeSlotTimer = new cMessage("antennaTimeSlot");
    std::vector<UserDescriptor*> users;
    std::vector<cMessage*> packetTimers;
    int networkDimension;
    int cqiMap[15] = {3,3,6,11,15,20,25,36,39,50,63,72,80,93,93};
    simtime_t period;
    Frame* frame;
    simtime_t packetMeanIntTime;
	simsignal_t throughputSignal;
	std::vector<simsignal_t> queueSizeSignals;

	simsignal_t createUserQueueSizeSignal(int);
    void handleCQIMessage(Cqi*);
    void handleExpInterrarival(cMessage*);
    void handleTimeSlot();
    static bool compareUsers(UserDescriptor* a,UserDescriptor* b) {
        if(a->getRCVBT() < b->getRCVBT()) return true;
        if(a->getRCVBT() == b->getRCVBT() && a->getID() <= b->getID()) return true;
        return false;
    }
    void resetFrame() {
        for(int i = 0; i < 25; ++i) frame->get_rbs(i)->reset();
    }
  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
  public:
    Antenna();
};

#endif
