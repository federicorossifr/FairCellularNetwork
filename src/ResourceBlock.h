#ifndef __FAIRCELLULARNETWORK_RESOURCEBLOCK_H_
#define __FAIRCELLULARNETWORK_RESOURCEBLOCK_H_

#include <omnetpp.h>
#include "Packet_m.h"
using namespace omnetpp;

class ResourceBlock {
	cQueue packets;
	int size;
	int available;
	int userID;
public:
	ResourceBlock() {;};
	~ResourceBlock() {;};
	int insertPacket(Packet* p);
	Packet* popPacket();
	int getSize(){return size;};
	int getAvailable(){return available;};
	int getUserID(){return userID;};
	void setSize(int s){size=s;};
    void setAvailable(int a){available=a;};
    void setUserID(int id){userID=id;};
};

#endif
