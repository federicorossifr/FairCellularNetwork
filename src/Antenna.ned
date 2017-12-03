simple Antenna
{
    parameters:
        int n;
	   	double timeSlotPeriod @unit(s);
	   	double packetMeanIntTime @unit(s);  
		@signal[throughput](source="throughput");
		@statistic[throughput](title="Throughput";record=vector);		
		
		@signal[user*queueSize]();
		@statisticTemplate[queueSize](record=vector);
    gates:
        input in[];
        output out[];
}
	