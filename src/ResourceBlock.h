#ifndef __FAIRCELLULARNETWORK_RESOURCEBLOCK_H_
#define __FAIRCELLULARNETWORK_RESOURCEBLOCK_H_

#include <omnetpp.h>
#include "Packet_m.h"
using namespace omnetpp;

class ResourceBlock {
	cQueue packets;
	int size=0;
	int available=0;
	int userID=-1;
public:
	ResourceBlock() {packets.clear();};
	~ResourceBlock() {};
	int insertPacket(Packet* p);
	Packet* popPacket();
	int getSize(){return size;};
	int getAvailable(){return available;};
	int getUserID(){return userID;};
	void setSize(int s){size=available=s;};
    void setAvailable(int a){available=a;};
    void setUserID(int id){userID=id;};
    bool isEmpty() {return (packets.getLength() == 0);}
    void reset() {packets.clear(); size=0; available=0; userID=-1;}
};

#endif
