simple User{
	parameters:
	   	int n;
	   	double timeSlotPeriod @unit(s); 
	   	int uniformServ; //0 Uniform 1 and 2 Binomial 
	   	int CqiRNGID = default(2);
	   	@signal[resp](source="resp");
    	@statistic[resp](title="responseTime";record=vector);
    	@signal[rcvd](source="rcvd");
    	@statistic[rcvd](title="receivedPacktes";record=count);
	gates:
	    input in;
	    output out;
}