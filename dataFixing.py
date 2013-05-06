fs = ['probe.txt','qualifying.txt']
gs = ['probe_tabbed.txt','qualifying_new.txt']

for i in range(2):
    f = open(fs[i], 'r')
    g = open(gs[i], 'w')


    for line in f.readlines():
        if line.find(':') != -1:
            index = line[:line.find(':')]
        else:
            line = line.replace('\n','')
            g.write(line + '\t' + index + '\n')
