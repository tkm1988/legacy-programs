# -*- coding: utf-8 -*-
import sys, math, random
from types import *
import numpy as np
import numexpr as ne
import scipy as sp

class SOM:    
    def __init__(self, x=0, y=0, data_dim=0):
	if x == 0 or y == 0:
	    raise ValueError, 'SOM is EMPTY.'

	if data_dim == 0:
	    raise ValueError, 'Do you know 0-dimension space?'

	self.__x = int(round(x))
	self.__y = int(round(y))
        self.__ltime = 0
        self.__ltime_limit = 0
        self.__alpha = 0.0
        self.__sigma = 0.0
        self.__w_normalize_flag = False
        self.__d_normalize_flag = False
        self.__Weight = np.zeros((self.__x*self.__y, data_dim))
        self.__Data = np.zeros((1,1))
        self.__Error = []
        self.__BrokenNeuron = []

	for i in xrange(x*y):
	    for j in xrange(data_dim):
		self.__Weight[i,j] = 10.0 * random.random()
            self.__BrokenNeuron.append(False) 

	print 'Init Weight : ', self.__Weight.view()

    def getAlpha(self):
        return self.__alpha

    def getSigma(self):
        return self.__sigma
    
    def getError(self):
        return self.__Error

    def getWeight(self):
        return self.__Weight

    def getBrokenNeuron(self):
        return self.__BrokenNeuron
    
    def setData(self, Data):
        if type(Data) is np.ndarray:
            self.__Data = Data
        else:
            raise TypeError, 'Data is not numpy.ndarray.'

    def setLearningTimeLimit(self, limit):
        self.__ltime_limit = int(round(limit))
        
    def Normalization(self, Target):
        if type(Target) is np.ndarray:
            for i in xrange(len(Target[:,0])):
                Target[i,:] = Target[i,:] / np.linalg.norm(Target[i,:])
        else:
            raise TypeError, 'Data is not numpy.matrix.'

    def ProdLearning(self, ltime=0, init_sigma=0.0, init_alpha=0.9, trace_mode=False):
        #ErrorCheck Phase-----------------------
        if ltime <= 0:
            raise ValueError,'This SOM is sluggard.'
        if init_alpha <= 0:
            raise ValueError,'If learning-rate coefficient is less than 0 , that means SOM don\'t learn never.'
        if init_sigma <= 0:
            raise ValueError,'If learning-extent is less than 0 , that means SOM don\'t learn never.'

        #PREP-----------------------------------
        if self.__w_normalize_flag is False:
            self.Normalization(self.__Weight)
            self.__w_normalize_flag = True
            for i in xrange(len(self.__BrokenNeuron)):
                if self.__BrokenNeuron[i] is True:
                    self.__Weight[i,:] = self.__Weight[i,:] * 0.0
            
        if self.__d_normalize_flag is False:
            self.Normalization(self.__Data)
            self.__d_normalize_flag = True

        print 'Normalized Weight : '
        print self.__Weight.view()
        print 'Normalized Data : '
        print self.__Data.view()
        #EXEC-----------------------------------
        if trace_mode is True:
            if self.__ltime_limit == 0:
                self.setLearningTimeLimit(ltime)
                
            tmp = 0.0
            self.__alpha = init_alpha *(0.01/init_alpha)**(self.__ltime / (self.__ltime_limit))
            self.__sigma = init_sigma *(0.01/init_sigma)**(self.__ltime / (self.__ltime_limit))
                                                           
            for i in random.sample(xrange(len(self.__Data[:,0])), len(self.__Data[:,0])):
                p = self.__Data[i,:]
                work = self.__Weight.dot(p.T).argmax()
                c = np.array(work/self.__x, work%self.__x)
                
                for j in xrange(len(self.__Weight[:,0])):
                    r = np.array([j / self.__x, j % self.__x])
                    h = self.__alpha * math.exp(-np.linalg.norm(ne.evaluate('c - r')) / (2*self.__sigma**2))
                    m = self.__Weight[j,:]
                    norm_val = np.linalg.norm(ne.evaluate('m + p * h'))
                    self.__Weight[j,:] = ne.evaluate('m + p * h / norm_val')

                m = self.__Weight[work,:]
                tmp = tmp + np.linalg.norm(ne.evaluate('m - p'))

            self.__Error.append(tmp)
            self.__ltime = self.__ltime + 1
        else:
            time = 0
            self.__sigma = init_sigma *(0.01/init_sigma)**(time / ltime)
            self.__alpha = init_alpha *(0.01/init_alpha)**(time / ltime)

            print 'time : '
            while time < ltime:
                print time
                tmp = 0.0
                for i in random.sample(xrange(len(self.__Data[:,0])), len(self.__Data[:,0])):
                    p = self.__Data[i,:]
                    work = self.__Weight.dot(p.T).argmax()
                    c = np.array(work/self.__x, work%self.__x)
                    
                    for j in xrange(len(self.__Weight[:,0])):
                        r = np.array([j / self.__x, j % self.__x])
                        h = self.__alpha * math.exp(-np.linalg.norm(ne.evaluate('c - r')) / (2*self.__sigma**2))
                        m = self.__Weight[j,:]
                        norm_val = np.linalg.norm(ne.evaluate('m + p * h'))
                        self.__Weight[j,:] = ne.evaluate('m + p * h / norm_val')
                        
                    m = self.__Weight[work,:]
                    tmp = tmp + np.linalg.norm(ne.evaluate('m - p'))
                        
                    self.__Error.append(tmp)
                    self.__sigma = init_sigma *(0.01/init_sigma)**(time / ltime)
                    self.__alpha = init_alpha *(0.01/init_alpha)**(time / ltime)
                self.__ltime = self.__ltime + 1
                time = time + 1
        print 'Finish :'
        print 'Weight :'
        print self.__Weight.view()
                
#    def Eval(self, input):
#        pass
#    
    def Broken(self, breakage_rate=0.0):
        self.__BrokenNeuron = []
        tmp = random.sample(0, self.__Weight.shape[0])
        for i in xrange(int(round(self.__Weight.shape[0] * breakage_rate * 0.01))):
            self.__BrokenNeuron[tmp[i]] = True
            
