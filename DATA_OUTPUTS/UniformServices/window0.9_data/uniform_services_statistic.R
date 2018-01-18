library("ineq");
library("ggplot2")
mean_rt_data <- scan("mean_response_data");
rt_data <- scan("response_time_data");
th_data <- scan("throughput_data");
qs_data <- scan("queue_size_data");
user_count <- 10;
mean_int_time <- 0.9;
timeslot_period <- 1;
sim_duration <- 120;
repetition <- 10;
warmup <- 30;
mode <- "Uniform CQI - Window ON alpha=0.4";
mean_int_rate <- timeslot_period/(mean_int_time);
pkt_rate_in <- user_count*mean_int_rate;
lcdata <- Lc(mean_rt_data);
rtLCG <- mad(mean_rt_data)/(2*mean(mean_rt_data));
scenario <- c(mode,"===============","User #:",user_count,"Timeslot [ms]:",timeslot_period,"Mean inter-arrival time [ms]:",mean_int_time,"Rate in [pkt/ms]:",pkt_rate_in);
scenario <- c(scenario,"Simulation time [s]:",sim_duration,"Warmup [s]:",warmup,"Repetitions:",repetition,"======================");
#RESPONSE TIME STATISTICS
rtStrings <- c(scenario,"Sample size:");
rtDataCount <- length(rt_data);
rtStrings <- c(rtStrings,rtDataCount);
rtDataMean <- mean(rt_data);
rtDataStd <- sqrt(var(rt_data));
rtMeanConfidenceInterval <- qnorm(0.995)*rtDataStd/sqrt(rtDataCount);
rtStrings <- c(rtStrings,"Mean [s]:",rtDataMean,"Confidence interval 99% [s]:",rtMeanConfidenceInterval);
rtStrings <- c(rtStrings,"Standard deviation [s]:",rtDataStd);
rtStrings <- c(rtStrings,"Lorenz curve gap:",rtLCG);
perUnFairness <- 1 - 1/rtDataCount;
rtStrings <- c(rtStrings,"Lorenz perfect unfairness gap: ",perUnFairness);

#THROUGHPUT STATISTICS
th_data <- th_data[th_data != 0];
thStrings <- c("Sample size:");
thDataCount <- length(th_data);
thStrings <- c(thStrings,thDataCount);
thDataMean <- mean(th_data);
thDataStd <- sqrt(var(th_data));
thMAD <- mean(abs(th_data - thDataMean));
thLCG <- thMAD/(2*thDataMean);
thMeanConfidenceInterval <- qnorm(0.995)*thDataStd/sqrt(thDataCount);
thStrings <- c(thStrings,"Mean [byte/s]:",thDataMean,"Confidence interval 99% [byte/s]:",thMeanConfidenceInterval);
thStrings <- c(thStrings,"Standard deviation [byte/s]:",thDataStd);
thStrings <- c(thStrings,"Lorenz curve gap:",thLCG);
perUnFairness <- 1 - 1/thDataCount;
thStrings <- c(thStrings,"Lorenz perfect unfairness gap: ",perUnFairness);

#QUEUE SIZE STATISTICS
qsDataMean <- mean(qs_data);

#Applying Little's law to extimate response time
expRt <- qsDataMean/(1000*pkt_rate_in);
rtStrings <- c(rtStrings,"Little response time extimation [s]:",expRt);
rtStatistics <- file("uniform_services_rt_computed_statistics.txt");
writeLines(rtStrings,rtStatistics);
close(rtStatistics);

thStatistics <- file("uniform_services_th_computed_statistics.txt");
writeLines(thStrings,thStatistics);
close(thStatistics);

png("response_time_lorenz_curve.png");
plot(lcdata);
dev.off();

