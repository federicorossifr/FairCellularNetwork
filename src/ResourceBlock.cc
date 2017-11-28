#include "resourceBlock.h"

ResourceBlock::ResourceBlock(int dim){
	_packets = new cQueue("RB",NULL);
	available = true;
	size = dim;
}

int ResourceBlock::insertPacket(Packet* p){
    int packetSize = p->getSize();
    if(packetSize <= available) {
        packets.insert(p);
        available-=packetSize;
        return available;
    }
    //Here packetSize > available
    if(p->getFragment()) { //If it is a fragment insert it
        packets.insert(p);
        available=0;
        return available;
    }
    //Notify caller that whole packet could not be inserted
    return -1;
}
