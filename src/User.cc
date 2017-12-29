#include "User.h"
#include "math.h"
int User::USER_COUNTER=0;
Define_Module(User);

void User::initialize()
{
    period = par("timeSlotPeriod");
    scheduleAt(simTime()+period,timeSlotTimer);
    resp_signal = registerSignal("resp");
    packet_signal = registerSignal("rcvd");
    throughput_signal = registerSignal("thr");
    served_signal = registerSignal("served");
}

int User::computeCqi() {
    int cqi;
    bool how = par("uniformServ");
    double sp = par("successProb");
    if(!how) //Uniform case
        cqi = uniform(1,15);
    else{
        int division = userID%2;
        double p = (division)?sp:1-sp;
        EV <<  p << endl;
        cqi = floor(binomial(14, p,par("CqiRNGID"))+1);
    }
    cqiSum+=cqi;
    cqiCount++;
    return cqi;
}

void User::handleFrame(Frame* frame) {
    EV << "Received a frame" << endl;
    long previousMsgId = -1;
    bool alreadyDone = false;
    //Scan the broadcast Frame
    int byteReceived = 0;
    for(int i=0; i<25; i++){
        //Check if the RB contains a packet that belongs to me.
        ResourceBlock* rb = frame->get_rbs(i);
        if(!rb->getSize()) continue;
        if(rb->getUserID() != userID && alreadyDone) break;
        if(rb->getUserID() != userID) continue;
        alreadyDone = true;
        Packet* p = NULL;
        while(!(rb->isEmpty())){
           p = rb->popPacket();
           if(previousMsgId == -1 || previousMsgId != p->getTreeId()) {
               byteReceived+=p->getSize();
               EV << "Extracted packet -- " << p->getTreeId() << " size -- " << p->getSize() << endl;
               simtime_t responseTime = simTime() - p->getCreation();
               EV << responseTime << endl;
               emit(packet_signal,1);
               emit(resp_signal,responseTime);
           }
           if(p->getFragment()) {
               previousMsgId = p->getTreeId();
           }
           else
               previousMsgId = -1;
           delete p;
         }
     }
     if(byteReceived > 0) emit(served_signal,1);
     emit(throughput_signal,byteReceived/period);
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

void User::finish() {
    EV << (double)(cqiSum)/(double)(cqiCount) << endl;
}
