simple User{
	parameters:
	   	int n;
	   	double timeSlotPeriod @unit(s); 
	   	int uniformServ; //0 Uniform 1 and 2 Binomial 
	   	int CqiRNGID = default(2);
	   	@signal[resp](source="resp");
    	@statistic[resp](title="responseTime";record=vector);
	gates:
	    input in;
	    output out;
}