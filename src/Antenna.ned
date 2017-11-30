simple Antenna
{
    parameters:
        int n;
	   	double timeSlotPeriod @unit(s);
	   	double packetMeanRate @unit(hz) = default(10hz);      
    gates:
        input in[];
        output out[];
}
	