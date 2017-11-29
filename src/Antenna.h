#ifndef __FAIRCELLULARNETWORK_ANTENNA_H_
#define __FAIRCELLULARNETWORK_ANTENNA_H_

#include <omnetpp.h>
#include <string>
#include "UserDescriptor.h"
#include "Cqi_m.h"
using namespace omnetpp;

class Antenna : public cSimpleModule
{
    cMessage* timeSlotTimer = new cMessage("timeSlot");
    UserDescriptor* users;
    cMessage* packetTimers;
    int networkDimension;

    void handleCQIMessage(Cqi*);
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
  public:
    Antenna() {
        //Retrieve network dimensions
        int dim = (int)par("n");
        networkDimension = dim;
        //Create an array of users descriptors
        users = new UserDescriptor[(int)par("n")];
        //Create array of Timers for user packets
        packetTimers = new cMessage[dim];
        //Initialize User Descriptors
        for(int i = 0; i < dim; ++i) {
            users[i].setID(i);
            packetTimers[i].setName( (std::to_string(i)).c_str() );
        }
    }
};

#endif
