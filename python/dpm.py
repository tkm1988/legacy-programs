import sys

if(len(sys.argv) != 3):
    print 'Argument Error :\nFor example,\n\t>python string1 string2\nPlease input me.'
    exit()

str1 = sys.argv[1]
str2 = sys.argv[2]
length1 = len(str1)
length2 = len(str2)
penalty_matrix = []
cost_matrix = []
x = 0
y = 0

while y < length2:
    ptmp = []
    ctmp = []
    while x < length1:
        if(str1[x] != str2[y]):
            ptmp.append(-2)
        else:
            ptmp.append(0)

        if(y == 0):
            if(x == 0):
                ctmp.append(['end', ptmp[0]])
            else:
                ctmp.append(['left', ctmp[x - 1][1] + ptmp[x] - 2])
        else:
            if(x == 0):
                ctmp.append(['up', cost_matrix[y - 1][x][1] + ptmp[x] - 2])
            else:
                i = 0
                itmp = 0
                ctmp1 = [['up', cost_matrix[y - 1][x][1] + ptmp[x] - 2]]
                ctmp1.append(['left-up', cost_matrix[y - 1][x - 1][1] + ptmp[x] - 1])
                ctmp1.append(['left', ctmp[x - 1][1] + ptmp[x] - 2])
                while i < 2:
                    if(ctmp1[itmp][1] <= ctmp1[i + 1][1]):
                        itmp = i
                    i = i + 1
                ctmp.append(ctmp1[itmp])
        x = x + 1
    penalty_matrix.append(ptmp)
    cost_matrix.append(ctmp)
    x = 0
    y = y + 1

print 'penalty matrix is'
for str in penalty_matrix:
    print str

print '\ncost matrix is'
for str in cost_matrix:
    print str

print '\nexit this process\n'
exit()
