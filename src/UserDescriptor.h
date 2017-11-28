#ifndef __FAIRCELLULARNETWORK_USERDESCRIPTOR_H_
#define __FAIRCELLULARNETWORK_USERDESCRIPTOR_H_

#include <omnetpp.h>
#include "Packet_m.h"
using namespace omnetpp;

class UserDescriptor {
	int userID;
	int receivedBytes;
	int currentCqi;
	cQueue packetQueue;

public:
	UserDescriptor() {;};
	virtual ~UserDescriptor() {;};
	void insertPacket(Packet* p);
	Packet* popPacket();
	
	void setID(int value);
	int getID();
	void setRCVBT(int rcv);
	int getRCVBT();
	void setCQI(int cqi);
	int getCQI();
};

#endif
