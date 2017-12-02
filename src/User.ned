simple User{
	parameters:
	   	int n;
	   	double timeSlotPeriod @unit(ms); 
	   	bool uniformServ; 
	gates:
	    input in;
	    output out;
}