simple User{
	parameters:
	    double successProb;
	   	int n;
	   	double timeSlotPeriod @unit(s); 
	   	bool uniformServ; //false Uniform true Binomial 
	   	int CqiRNGID = default(2);
	   	@signal[resp](source="resp");
    	@statistic[resp](title="responseTime";record=vector,mean);
    	@signal[rcvd](source="rcvd");
    	@statistic[rcvd](title="receivedPacktes";record=count);
    	@signal[thr](source="thr");
    	@statistic[thr](title="Throughput";record=vector,sum);
	gates:
	    input in;
	    output out;
}