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
    //scan the broadcast RBs frame arrived
    for(int i=0; i<25; i++){
        //check if the current RB contains packet that belong to me, otherwise skip it
        ResourceBlock* rb = frame->get_rbs(i);
        if(rb->getUserID() != userID) continue;
        Packet* p = NULL;
        while( (p = check_and_cast<Packet *>(rb->popPacket())) ){
        //check if the current packet is completely content in the current RB
            if(!p->getFragment()){
                simtime_t responseTime = simTime() - p->getCreation();
                //TODO - save response time with signal
                delete p;
                //check if the previous packet in the current RB was a "last fragment" of a packet bigger than user's RB size
                if(fragmentMessage != NULL){
                    simtime_t responseTime = simTime() - fragmentMessage->getCreation();
                    //TODO - save response time with signal
                    delete fragmentMessage;
                    fragmentMessage = NULL;
                    previousMsgId = -1;
                }
             }else{
                 //the previous packet was all content in the RB
                 if(previousMsgId == -1){
                     previousMsgId = p->getId();
                     fragmentMessage = p;
                 } else if(previousMsgId != p->getId()){
                     //check if this fragment is a fragment of the previous fragmented packet or a fragment of a new one
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
