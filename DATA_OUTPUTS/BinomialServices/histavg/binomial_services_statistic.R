library("ineq");
library("ggplot2")

rt_data <- scan("response_time_data");

user_count <- 10;
mean_int_time <- 1.3;
timeslot_period <- 1;
sim_duration <- 120;
repetition <- 100;
warmup <- 30;
mode <- "Binomial CQI";
mean_int_rate <- timeslot_period/(mean_int_time);
pkt_rate_in <- user_count*mean_int_rate;
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

rtStatistics <- file("uniform_services_rt_computed_statistics.txt");
writeLines(rtStrings,rtStatistics);
close(rtStatistics);

