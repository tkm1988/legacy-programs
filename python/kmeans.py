import sys, string
import scipy.cluster.vq
import numpy as np

array = []
clsOne = []
clsTwo = []
for line in open('ArtiData.txt','r'):
    tmp = []
    count = 0
    startPoint = 0
    for i in xrange(len(line)):
        if line[i] == ' ' and count < 3:
            tmp.append(int(line[startPoint:i]))
            startPoint = i + 1
            count = count + 1
    array.append(tmp)
    if line[len(line) - 2] == '1':
        clsOne.append(tmp)
    else :
        clsTwo.append(tmp)

obs = np.array(array)

np.savetxt('clsOne.txt', np.array(clsOne))
np.savetxt('clsTwo.txt', np.array(clsTwo))
np.savetxt('k_1.txt', scipy.cluster.vq.kmeans(obs, 1)[0])
np.savetxt('k_2.txt', scipy.cluster.vq.kmeans(obs, 2)[0])
np.savetxt('k_3.txt', scipy.cluster.vq.kmeans(obs, 3)[0])
np.savetxt('k_4.txt', scipy.cluster.vq.kmeans(obs, 4)[0])
