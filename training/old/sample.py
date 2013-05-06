import random

r = random
rands = []
for i in range(100):
    rands.append(int(r.random()*100480507))

rands.sort()

f = open('ratings.txt', 'r')
g = open('sample.txt', 'w')

i = 0
j = 0

for line in f.readlines():
    if rands[j] == i:
        g.write(line)
        j+=1
    i+=1

