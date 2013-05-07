#!/usr/bin/perl
# A simple implementation of the RMSE calculation used for the Netflix Prize
use strict;
use warnings;
my $numValues = 0;
my $sumSquaredValues = 0;
open(DATA, "<training/output");
while (<DATA>) {
    my ($new_movie,$new_mean,$new_sd,$prediction,
	$movie,$mean, $sd,$old_rating,$rating,$customer_id) = split(/\,/);
    if($prediction < 1){
	$prediction = 1;
    }
    if($prediction > 5){
	$prediction = 5;
    }
    my $delta = $rating - $prediction;
    $numValues++;
    $sumSquaredValues += $delta*$delta;
#    printf("%d rating, %.5f predicted\n", $rating, $prediction);
}
# Let perl do the truncation to 0.0001
printf "%d pairs RMSE: %.5f\n", $numValues, sqrt($sumSquaredValues/$numValues);

# Some example data rating & prediction data
# NOTE: This is NOT in the proper prize format
#__DATA__
#2,3.2
#3,3.1
#4,5.0
