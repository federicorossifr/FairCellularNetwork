#include "User.h"
int User::USER_COUNTER=0;
Define_Module(User);

void User::initialize()
{
	timeSlotTimer->setSchedulingPriority(1111);
    scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
}

int User::computeCqi() {
    int cqi = 1;
    cqi = uniform(1,15);
    return cqiMap[cqi-1];
}

void User::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
    	//send CQI to the Antenna 
        send(computeCqi(),"out");
        scheduleAt(simTime()+par("timeSlotPeriod"),timeSlotTimer);
    }
    else{
    	long previousMsgId = 0;
    	Packet* fragmentMessage = NULL;

    	//scan the broadcast RBs frame arrived
    	for(int i=0; i<25; i++){
    		//check if the current RB contains packet that belong to me, otherwise skip it
    		if(msg->_rsb[i].getUserID() == userID){
    			Packet* p = NULL;
    			While(p = check_and_cast<Packet *>(msg->_rsb[i].popPacket())){
    				//check if the current packet is completely content in the current RB
    				if(!p->fragment){
    					simtime_t responseTime = simTime() - p->creation;
    					//TODO - save response time with signal
    					delete p;

    					//check if the previous packet in the current RB was a "last fragment" of a packet bigger than user's RB size
    					if(fragmentMessage != NULL){
    						simtime_t responseTime = simTime() - fragmentMessage->creation;
    						//TODO - save response time with signal
    						delete fragmentMessage;

    						fragmentMessage = NULL;
    						previousMsgId = 0;
    					}
    				}
    				else{
    					//the previous packet was all content in the RB
    					if(previousMsgId == 0){
    						previousMsgId = p.getId();
    						fragmentMessage = p;
    					}
    					//check if this fragment is a fragment of the previous fragmented packet or a fragment of a new one
    					else if(previousMsgId != p.getId()){
    						simtime_t responseTime = simTime() - fragmentMessage->creation;
    						//TODO - save response time with signal
    						delete fragmentMessage;

    						fragmentMessage = p;
    						previousMsgId =  p.getId();
    					}
    				}
    			}
    		}
    	}

    }
}
