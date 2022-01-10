import numpy as np
import random as rnd
import copy

def sgn(num):
    return (1 if(num > 0.0) else (0 if(num == 0.0) else -1))

class AssociativeMemory:
    def __init__(self, patternNum=1):
        self.__patternNum = patternNum
        self.__pattern = []

        tmpArray = []
        lineNum = 0
        for line in open('ferro_T215','r'):
            if lineNum >= 2:
                line.rstrip('\n')
                for charNum in xrange(len(line)):
                    if ((line[charNum] == "1") or (line[charNum] == "0")):
                        tmpArray.append(1.0 if (int(line[charNum]) == 1) else -1.0)
            lineNum = lineNum + 1
        self.__size = len(tmpArray)
        self.__lena = np.array(tmpArray)
        
        for i in xrange(self.__size):
            tmpNDArray = np.zeros(self.__patternNum)
            tmpNDArray[0] = self.__lena[i]
            for k in xrange(1, self.__patternNum):
                tmpNDArray[k] = (1.0 if(rnd.random() > 0.5) else -1.0)
            self.__pattern.append(tmpNDArray)

        self.__weight = np.zeros((self.__size, self.__size))
        for i in xrange(self.__size):
            for j in xrange(i, self.__size):
                if i != j :
                    self.__weight[i][j] = np.dot(self.__pattern[i], (self.__pattern[j]).T) / self.__size
                    self.__weight[j][i] = self.__weight[i][j]
                else:
                    self.__weight[i][j] = 0.0
                    self.__weight[j][i] = 0.0
        
    def recall(self, noiseRate = 0.0, step = 15):
        state = copy.deepcopy(self.__lena)
        self.__m = []

        for i in xrange(len(state)):
            if noiseRate > 0.0:
                state[i] = (state[i] if(rnd.random() > noiseRate)\
                            else (1.0 if(state[i] == -1)\
                                  else -1.0))

        prodResult = np.dot(state, self.__lena.T)
        self.__m.append(prodResult / (self.__size))
        
        for t in xrange(step):
            nextState = np.zeros(self.__size)

            for i in xrange(self.__size):
                prodResult = np.dot(self.__weight[i], state.T)
                nextState[i] = sgn(prodResult)

            prodResult = np.dot(nextState, self.__lena.T)
            self.__m.append(prodResult / (self.__size))
            state = copy.deepcopy(nextState)

        return self.__m
