simple User{
	parameters:
	   	int n;
	   	double timeSlotPeriod @unit(s); 
	   	bool uniformServ; 
	   	@signal[resp](source="resp");
    	@statistic[resp](title="responseTime";record=mean);
	gates:
	    input in;
	    output out;
}