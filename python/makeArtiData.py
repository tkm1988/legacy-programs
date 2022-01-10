import string,random
import numpy as np

data = []

for i in xrange(1000):
    if random.random() > 0.5 :
        data.append([int(255 * random.random()), int(10 * random.random()), int(5 * random.random()),1])
    else :
        data.append([int(10 * random.random()), int(5 * random.random()), int(255 * random.random()),2])

print data

np.savetxt('ArtiData.txt', np.array(data), '%d')
