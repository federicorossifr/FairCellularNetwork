#include "ResourceBlock.h"

int ResourceBlock::insertPacket(Packet* p){
    int packetSize = p->getSize()-p->getPackedSize();
    if(packetSize <= available) {
        packets.insert(p);
        if(p->getFragment()) p->setPackedSize(p->getSize());
        available-=packetSize;
        return (available);
    }
    //Here packetSize > available
    if(p->getFragment()) { //If it is a fragment insert it
        p->setPackedSize(p->getPackedSize()+available);
        packets.insert(p);
        available=0;
        return (available);
    }
    //Notify caller that whole packet could not be inserted
    return (-1);
}

Packet* ResourceBlock::popPacket(){
    return (check_and_cast<Packet *>(packets.pop()));
}
