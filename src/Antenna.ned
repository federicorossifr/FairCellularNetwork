simple Antenna
{
    parameters:
        int n;
	   	double timeSlotPeriod @unit(s);
	   	double packetMeanIntTime @unit(s);   
    gates:
        input in[];
        output out[];
}
	