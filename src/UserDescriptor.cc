#include "UserDescriptor.h"
void UserDescriptor::insertPacket(Packet* p){packetQueue.insert(p);}

Packet* UserDescriptor::popPacket(){return (Packet*)packetQueue.pop();}

void UserDescriptor::setID(int value){userID = value;}

int UserDescriptor::getID(){return userID;}

void UserDescriptor::setRCVBT(int rcv){receivedBytes += rcv;}

int UserDescriptor::getRCVBT(){return receivedBytes;}

void UserDescriptor::setCQI(int cqi){currentCqi = cqi;}

int UserDescriptor::getCQI(){return currentCqi;}
