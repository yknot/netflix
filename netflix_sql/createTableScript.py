
# form of file (customer, rating, date)

f = open('sqlTableScript', 'w')

f.write('USE netflix;\n')

for i in range(1,17771):
    digi = len(str(i))
    zero = 7-digi
    zeroes = '0' * zero
    table = 'mv_'+ zeroes + str(i)
    f.write('CREATE TABLE ' + table + ' (customer INT(7), rating INT(1), date DATE);\n')
