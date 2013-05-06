
# anything with ## was run previously and saved in .RData to cut down
# on time to run this script

## ratings <- read.csv("ratings.txt", header=F)
load(".RData")

hist.stats <- hist(ratings$V2, plot = FALSE)

len <- length(ratings$V2)

# plot a pie chart of the ratings across all the movies
# define the slices
slices <- c(hist.stats$counts[1], hist.stats$counts[5], hist.stats$counts[10], hist.stats$counts[15], hist.stats$counts[20])
# define the labels
lbls <- c("1", "2", "3", "4", "5")
#plot the pie
pie(slices, labels = lbls, main="Pie Chart of Ratings Distribution")


# plot number of ratings by average rating
## customers.id <- unique(ratings$V1)
#len <- length(customers.id)
#customers.avg.rating <- rep(NA, len)
#customers.num.rating <- rep(NA, len)
#for( i in 1:len){
#  customers.avg.rating[i] <- mean(ratings$V2[ratings$V1 == customers.id[i]])
#  customers.num.rating[i] <- length(ratings$V2[ratings$V1 == customers.id[i]])
#}
#plot(customers.avg.rating, customers.num.rating, main = "Customer Movie Rating by Movies Rated", xlab = "Average Movie Rating", ylab = "Number of Movies Rated")
