f = open('ratings.txt', 'w')

for i in range(1,17771):
    digi = len(str(i))
    zero = 7-digi
    zeroes = '0' * zero
    file = 'mv_' + zeroes + str(i) + '.txt'
    g = open(file, 'r')
    g.readline()
    for line in g.readlines():
        line = line[:len(line)-1]
#        print line
        f.write(line + ',' + str(i) + '\n')
