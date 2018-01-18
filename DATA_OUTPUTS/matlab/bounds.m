function [ upperBound, lowerBound ] = bounds(population)
%this function returns the bounds of the 95th percentile of the "population" 
%with the specified 99% confidence
j = 88;
k = 100;
ordered = sort(population);
upperBound = ordered(k)
lowerBound = ordered(j)
end

