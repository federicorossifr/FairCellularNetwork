simple Antenna
{
    parameters:
        int n;
        bool windowing = default(false);
        double windowingWeight = default(0.5);
	   	double timeSlotPeriod @unit(s);
	   	double packetMeanIntTime @unit(s);  
	   	int exponentialIntArrRNGID = default(0);
	   	int exponentialSizeRNGID = default(1);	   	
	   	int timeslotWindow = default(-1);
		@signal[throughput](source="throughput");
		@statistic[throughput](title="Throughput";record=mean);		
		@signal[packetsPerSlot]();
		@statistic[packetsPerSlot](title="Packet per slot";record=mean);		
		
		@signal[packetsSent]();
		@statistic[packetsSent](title="Packets sent";record=count);
		
		@signal[queueingTime]();
		@statistic[queueingTime](title="Queueing time";record=vector);
		
		@signal[queuedPacketsPerSlot]();
		@statistic[queuedPacketsPerSlot](title="Queued packets";record=mean);

		@signal[emptyRB]();
		@statistic[emptyRB](title="Empty Resource Blocks";record=mean);

		@signal[user*queueSize]();
		@statisticTemplate[queueSize](record=mean);
    gates:
        input in[];
        output out[];
}
	