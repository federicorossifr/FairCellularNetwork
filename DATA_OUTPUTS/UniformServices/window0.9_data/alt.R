library("ineq");
library("ggplot2");
data <- scan("mean_response_data");
reps <- matrix(data,nrow=10,ncol=100);
png("alternativePlot.png");
max <- seq(from=0,to=1,by=1/10);
lcgs <- c();
lcdatasums <- rep(0,11);
plot(max,max,type='l',xlim=c(0,1),ylim=c(0,1),xaxs="i",yaxs="i",ylab="",main="100 reps Lorenz curve");
for(col in 1:100) {
	sorted <- sort(reps[,col]);
	lcdata <- cumsum(sorted)/sum(sorted);
	lcdata <- c(0,lcdata);
	lcdatasums <- lcdatasums + lcdata;
	lcg <- mad(reps[,col])/(2*mean(reps[,col]));
	lcgs <- c(lcg,lcgs);
	lines(max,lcdata,col="Gray",lwd=1);
}
lcdatameans <- lcdatasums/100;
lcci <- qnorm(0.995)*sqrt(var(lcgs))/sqrt(length(lcgs));
lines(max,lcdatameans,col="Red",lwd=3);
dev.off();
maxLCGout <- file("lcgout.txt");
writeLines(c("Max:",max(lcgs),"Mean:",mean(lcgs),"CI:99%:",lcci),maxLCGout);
close(maxLCGout);