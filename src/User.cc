#include "User.h"
#include "math.h"
int User::USER_COUNTER=0;
Define_Module(User);

void User::initialize()
{
    period = par("timeSlotPeriod"); //Time elapsed from two CQI generation 
    scheduleAt(simTime()+period,timeSlotTimer); //Signal associated to the trasmission of a CQI to the antenna

    //Statistics registration
    resp_signal = registerSignal("resp"); //User's response time 
    packet_signal = registerSignal("rcvd");//Number of packet received by the user
    throughput_signal = registerSignal("thr");//User's throughtpu
    served_signal = registerSignal("served");//Number of "useful" timeslot for the user
}

int User::computeCqi() {
    int cqi;
    bool how = par("uniformServ");//Parameter used to set the way CQI are generated (0 = Uniform; 1 = Binomial)
    double sp = par("successProb");//Success probability for Binomial case
    if(!how) //Uniform case
        cqi = uniform(1,15);
    else{
        int division = userID%2;
        double p = (division)?sp:1-sp;//A probability significantly different is associated to even and odd UserID
        EV <<  p << endl;
        cqi = floor(binomial(14, p,par("CqiRNGID"))+1);
    }
    //The follow two variable are used to calculate the CQI mean value at the end of the simulation
    cqiSum+=cqi;
    cqiCount++;
    return cqi;
}

/*
* This method is used to let the user catch packet that belongs to him from the Frame sended by the Antenna on each timeslot
*/
void User::handleFrame(Frame* frame) {
    EV << "Received a frame" << endl;
    long previousMsgId = -1;//Variable used to check if the previous packet was a fragment of a bigger one
    bool alreadyDone = false;//Variable use to check if the current user have already read his own packet in the Scanning of the frame
    
    //Scan the broadcast Frame
    int byteReceived = 0;
    for(int i=0; i<25; i++){
        //Check if the RB contains a packet that belongs to me.
        ResourceBlock* rb = frame->get_rbs(i);
        if(!rb->getSize()) continue;//Error detection
        if(rb->getUserID() != userID && alreadyDone) break;//Packets of one user are adjacent in the Frame
        if(rb->getUserID() != userID) continue;
        alreadyDone = true;
        Packet* p = NULL;
        while(!(rb->isEmpty())){//Scan the current RB
           p = rb->popPacket();
           if(previousMsgId == -1 || previousMsgId != p->getTreeId()) {//If the previous packet was not a fragment or if the packet ID is different, the current packet is a new one
               byteReceived+=p->getSize();
               EV << "Extracted packet -- " << p->getTreeId() << " size -- " << p->getSize() << endl;
               simtime_t responseTime = simTime() - p->getCreation();
               EV << responseTime << endl;
               emit(packet_signal,1);
               emit(resp_signal,responseTime);
           }
           if(p->getFragment()) {//If the current packet is a fragment of a bigger one I have to have memory of its packet ID to perform the previou check
               previousMsgId = p->getTreeId();
           }
           else
               previousMsgId = -1;
           delete p;
         }
     }
     if(byteReceived > 0) emit(served_signal,1);//"Useful" time slot
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
        cqiMsg->setSchedulingPriority(1111);//CQI trasmission must be executed first to let the Antenna build the Frame in a proper way
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
    EV << (double)(cqiSum)/(double)(cqiCount) << endl;//Calculate CQI simulation mean value 
}
