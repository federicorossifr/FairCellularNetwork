simple Antenna
{
    parameters:
        int n;
	   	double timeSlotPeriod @unit(ms);
	   	double packetMeanIntTime @unit(ms);   
    gates:
        input in[];
        output out[];
}
	