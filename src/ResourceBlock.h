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
};

#endif
