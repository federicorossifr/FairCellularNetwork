#include "ResourceBlock.h"

int ResourceBlock::insertPacket(Packet* p){
    int packetSize = p->getSize()-p->getPackedSize();
    if(packetSize <= available) {
        packets.insert(p);
        available-=packetSize;
        return available;
    }
    //Here packetSize > available
    if(p->getFragment()) { //If it is a fragment insert it
        p->setPackedSize(available);
        packets.insert(p);
        available=0;
        return available;
    }
    //Notify caller that whole packet could not be inserted
    return -1;
}

Packet* ResourceBlock::popPacket(){
    return packets.pop();
}
