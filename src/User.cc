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
    bool alreadyDone = false;
    //Scan the broadcast Frame
    for(int i=0; i<25; i++){
        //Check if the RB contains a packet that belongs to me.
        ResourceBlock* rb = frame->get_rbs(i);
        if(rb->getUserID() != userID && alreadyDone) break;
        if(rb->getUserID() != userID) continue;
        alreadyDone = true;
        Packet* p = NULL;
        while(!(rb->isEmpty())){
           if(previousMsgId == -1 || previousMsgId != p->getTreeId()) {
               simtime_t responseTime = simTime() - p->getCreation();
               // TODO - Emit response time to signal.
           }
           if(p->getFragment())
               previousMsgId = p->getTreeId();
           else
               previousMsgId = -1;
           delete p;
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
