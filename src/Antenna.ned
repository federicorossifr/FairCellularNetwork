simple Antenna
{
    parameters:
        int n;
	   	double timeSlotPeriod @unit(s);
	   	double packetMeanIntTime @unit(s);  
	   	int exponentialIntArrRNGID = default(0);
	   	int exponentialSizeRNGID = default(1);	   	
		@signal[throughput](source="throughput");
		@statistic[throughput](title="Throughput";record=vector);		
		@statistic[bytesPerSlotSignal](title="bytesPerSlotSignal";record=vector);		
		
		@signal[user*queueSize]();
		@signal[bytesPerSlotSignal]();
		@statisticTemplate[queueSize](record=vector);
    gates:
        input in[];
        output out[];
}
	