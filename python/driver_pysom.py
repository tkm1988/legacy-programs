# -*- coding: utf-8 -*-
import sys, math, random
from types import *
import numpy as np
import numexpr as ne
import scipy as sp

import PySom

f = open('iris.dat','r')
item = []

for line in f:
    item.append(line[:-7].split('\t'))

Data = np.array(item,dtype=float)

print 'Data :'
print len(Data[:,0]), len(Data[0,:])

s = PySom.SOM(10,10,len(Data[0,:]))
s.setData(Data)

s.ProdLearning(1000, len(Data[:,0])/2)
