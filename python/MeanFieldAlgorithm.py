import sys, copy, string, math, random
import numpy as np

q = 0.2
h = 0.5 * math.log((1-q)/q)
J = 0.4
th = 0.0001

currentError = float(sys.maxint)
reconstructRate = 0
errorList = []
baseBitString = []
degBitString = []
newDeg = []
oldDeg = []
baseDeg = [] 
base = []

lineNum = 1
for line in open('ferro_T215','r'):
    if lineNum >= 3:
        line.rstrip('\n')
        for charNum in xrange(len(line)):
            if ((line[charNum] == "1") or (line[charNum] == "0")):
                baseBitString.append(1 if (int(line[charNum]) == 1) else -1)
                if (random.random() < q):
                    degBitString.append((-1 if (int(line[charNum]) == 1) else 1))
                else :
                    degBitString.append(1 if (int(line[charNum]) == 1) else -1)
                
    lineNum = lineNum + 1

for i in xrange(50, len(baseBitString) - 50, 50):
    base.append(baseBitString[i:i+50])
    baseDeg.append(degBitString[i:i+50]) 

for i in xrange(len(baseDeg)):
    tmp = []
    for j in xrange(len(baseDeg[i])):
        newData = math.tanh(J * ((baseDeg[i][j+1] if ((j + 1) < len(baseDeg[i])) else 0)\
                                 + (baseDeg[i][j-1] if ((j - 1) >= 0) else 0)\
                                 + (baseDeg[i+1][j] if ((i + 1) < len(baseDeg)) else 0)\
                                 + (baseDeg[i-1][j] if ((i - 1) >= 0) else 0)) + h * baseDeg[i][j])
        tmp.append(newData)
    oldDeg.append(tmp)

print 'Degeneration Image :'
for i in xrange(len(baseDeg)):
    print str(baseDeg[i])

while (currentError > th):
    currentError = 0.0;
    for i in xrange(len(baseDeg)):
        tmp = []
        for j in xrange(len(baseDeg[i])):
            newData = math.tanh(J * ((oldDeg[i][j+1] if ((j + 1) < len(oldDeg[i])) else 0)\
                                   + (oldDeg[i][j-1] if ((j - 1) >= 0) else 0)\
                                   + (oldDeg[i+1][j] if ((i + 1) < len(oldDeg)) else 0)\
                                   + (oldDeg[i-1][j] if ((i - 1) >= 0) else 0)) + h*baseDeg[i][j])
            currentError = currentError + abs(newData - oldDeg[i][j])
            tmp.append(newData)
        newDeg.append(tmp)
    oldDeg = copy.deepcopy(newDeg)
    errorList.append(currentError)
    del newDeg[:]

for i in xrange(len(oldDeg)):
    for j in xrange(len(oldDeg[i])):
        oldDeg[i][j] = (1 if (oldDeg[i][j] > 0) else (0 if (oldDeg[i][j] == 0) else -1))
        reconstructRate = reconstructRate + (1 if (abs(base[i][j] - oldDeg[i][j]) == 0) else 0)

print 'Reconstruct Image:'
for i in xrange(len(oldDeg)):
    print str(oldDeg[i])

if (float(len(base) * len(base[0])) > 0.0):
    print 'Reconstruct Rate : \n \t' + str(reconstructRate) + '/' + str(float(len(base) * len(base[0]))) + \
          ' = ' + str(reconstructRate/float(len(base) * len(base[0])))

for i in xrange(len(oldDeg)):
    for j in xrange(len(baseDeg[i])):
        baseDeg[i][j] = (baseDeg[i][j] if (baseDeg[i][j] == 1) else 0)

for i in xrange(len(oldDeg)):
    for j in xrange(len(oldDeg[i])):
        oldDeg[i][j] = (oldDeg[i][j] if (oldDeg[i][j] == 1) else 0)

np.savetxt('hoge.txt',np.array(baseDeg), '%d', ' ')
np.savetxt('fuga.txt',np.array(oldDeg), '%d', ' ')
