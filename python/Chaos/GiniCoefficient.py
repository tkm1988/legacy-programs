#!/usr/bin/env python

import sys,string
import numpy as np

inputData = []
for line in open('kadai3DATA','r'):
    inputData.append(float(line.rstrip('\r\n')))

inputData.sort()
N = len(inputData)
x = np.array(inputData)
mean = np.mean(x)
tmp = []
for i in xrange(N):
    tmp.append(2 * i - N - 1)

val = np.array(tmp)
gini = np.dot(val, x) / (N**2 * mean)

print "Gini Index : " + str(gini)
