simple User{
	parameters:
	   	int n;
	   	double timeSlotPeriod @unit(s); 
	   	bool uniformServ; 
	gates:
	    input in;
	    output out;
}