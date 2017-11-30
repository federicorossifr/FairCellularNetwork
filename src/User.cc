#include "User.h"
int User::USER_COUNTER=0;
Define_Module(User);

void User::initialize()
{
    scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
}

int User::computeCqi() {
    int cqi = 1;
    cqi = uniform(1,15);
    return cqi;
}

void User::handleFrame(Frame* frame) {
    long previousMsgId = -1;
    Packet* fragmentMessage = NULL;
    //Scan the broadcast Frame
    for(int i=0; i<25; i++){
        //Check if the RB contains a packet that belongs to me.
        ResourceBlock* rb = frame->get_rbs(i);
        if(rb->getUserID() != userID) continue;
        Packet* p = NULL;
        while( (p = rb->popPacket()) ){
            //Check if the current packet is not fragmented
            if(!p->getFragment()){
                simtime_t responseTime = simTime() - p->getCreation();
                //TODO - Save response time with signal
                delete p;
                //Check if the previous packet in the current RB was the last fragment for a bigger packet
                if(fragmentMessage != NULL){
                    simtime_t responseTime = simTime() - fragmentMessage->getCreation();
                    //TODO - Save response time with signal
                    delete fragmentMessage;
                    fragmentMessage = NULL;
                    previousMsgId = -1;
                }
             }else{
                 //The previous packet was a fragment
                 if(previousMsgId == -1){
                     previousMsgId = p->getId();
                     fragmentMessage = p;
                   //Check if this is a fragment for a different packet
                 } else if(previousMsgId != p->getId()){
                     simtime_t responseTime = simTime() - fragmentMessage->getCreation();
                     //TODO - save response time with signal
                     delete fragmentMessage;
                     fragmentMessage = p;
                     previousMsgId =  p->getId();
                 }
             }
         }
     }
     delete frame;
}

void User::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
    	//send CQI to the Antenna 
        Cqi* cqiMsg = new Cqi();
        cqiMsg->setCqiValue(computeCqi());
        cqiMsg->setUserID(userID);
        cqiMsg->setSchedulingPriority(1111);
        cqiMsg->setName("CQI");
        send(cqiMsg,"out");
        scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
    }
    else{
        Frame* frame = check_and_cast<Frame *>(msg);
    	handleFrame(frame);
    }
}
