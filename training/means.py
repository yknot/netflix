
# function to calc std dev
def stddev(nums, mean):
    sum = 0
    # calculate summation
    for i in nums:
        sum += (i - mean)**2
    
    return (sum/n)**.5



# open the file to write the results to
f = open("mean_ratings.txt", 'w')

# for loop through all the seperate movie files
for i in range(1,17771):
    # get the number of zeros in the file name
    digi = len(str(i))
    zero = 7-digi
    zeroes = '0' * zero

    # concatinate the file name
    file = 'files/mv_' + zeroes + str(i) + '.txt'

    # open the file
    g = open(file, 'r')

    # read the first line that shows the movie number
    g.readline()

    # set up the counter variables
    sum = 0.0
    n = 0.0
    bins = [0]*5
    nums = []

    # for each line in the file
    for line in g.readlines():
        # remove the new line character
        line = line[:len(line)-1]
        # split the line by commas
        line = line.split(',')
        # at the rating to the sum
        sum += int(line[1])
        # increment the number of ratings
        n += 1
        # increment the right bin
        bins[int(line[1])-1]+=1
        # append value for sd calc
        nums.append(int(line[1]))

    mean = sum/n
    # calc the sd
    sd = stddev(nums, mean)
    
    # write to the output file 
    f.write(str(i) + ',')
    for x in bins:
        f.write(str(x) + ',')

    f.write(str(mean) + ',' + str(sd) + ',' + str(n) + '\n')
