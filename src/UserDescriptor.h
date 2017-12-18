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
	UserDescriptor();
	virtual ~UserDescriptor() {};
	void insertPacket(Packet* p);
	Packet* popPacket();
	void undoPopPacket(Packet* p);
	void setID(int value);
	int getID();
	void setRCVBT(int rcv);
    void setRCVBT(int rcv,double alpha);
	void resetRCVBT() {receivedBytes=0;}
	int getRCVBT();
	void setCQI(int cqi);
	int getCQI();
	bool hasPacket() {return (packetQueue.getLength() > 0);}
	int packetCount() {return packetQueue.getLength();}
};

#endif
