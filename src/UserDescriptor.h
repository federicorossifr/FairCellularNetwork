#ifndef __FAIRCELLULARNETWORK_USERDESCRIPTOR_H_
#define __FAIRCELLULARNETWORK_USERDESCRIPTOR_H_

#include <omnetpp.h>

using namespace omnetpp;

class UserDescriptor
{
private:
	int userID;
	int receivedBytes;
	int currentCqi;
	cQueue packetQueue;

	bool insertPacket(Packet* p);
	Packet* popPacket();

	virtual UserDescriptor() {;};
	virtual ~UserDescriptor() {;};
	
	void setID(int value);
	int getID();
	void setRCVBT(int rcv);
	int getRCVBT();
	void setCQI(int cqi);
	int getCQI();
};

#endif
