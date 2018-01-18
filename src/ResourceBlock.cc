#include "ResourceBlock.h"
/*
 * This method tries to insert a packet in the resource block
 * if the available size is enough packet will be inserted
 * and remaining size will be returned.
 * Otherwhise packet will be rejected, until it is marked as
 * fragment.
 */
int ResourceBlock::insertPacket(Packet* p){
    int packetSize = p->getSize()-p->getPackedSize();
    if(packetSize <= available) {
        packets.insert(p);
        //Last fragment inserted, packed size is the overall size
        if(p->getFragment()) p->setPackedSize(p->getSize());
        available-=packetSize;
        return (available);
    }
    //Here packetSize > available
    //If it is a fragment insert it and update the current packed size of the packet
    if(p->getFragment()) {
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
