import sys, math, random, scipy, psyco
from random import *
from scipy import *
from scipy.linalg import *
from scipy import io

def som(inputVector, Dimention, learningTime, init_eta):
	#error processing
	if(type(inputVector) is not type(array([]))):
		print "ERROR! InputVector : Invalid Value."
		return 0, 0
	
	if((Dimention < 1 & Dimention > 2) | (type(Dimention) is not type(0))):
		print "ERROR! Dimention : Invalid Value."
		return -1, -1
	
	if(learningTime < 1 | (type(learningTime) is not type(0))):
		print "ERROR! LearningTime : Invalid Value."
		return -2, -2
	
	#initialize variable
	inputVectorRowNum = inputVector.shape[0]
	try:
		inputVectorColumnNum = inputVector.shape[1]
	except:
		inputVectorColumnNum = 0

	#refarence vector
	if(Dimention == 1):
		Ref = rand(inputVectorRowNum, inputVectorColumnNum)
		
		#standard variation
		init_sigma = floor(inputVectorRowNum/2)
	else:
		if((math.sqrt(inputVectorRowNum) % 1.0) != 0.0): 
			xlength = int(floor(inputVectorRowNum/2))
			Ref = rand(xlength**2, inputVectorColumnNum)
			init_sigma = floor(floor(inputVectorRowNum/2)**2/2)
		else:
			Ref = rand(inputVectorRowNum, inputVectorColumnNum)
			xlength = int(math.sqrt(inputVectorRowNum))
			init_sigma = floor(inputVectorRowNum/2)

	ylength = xlength
	sigma = init_sigma
	refVectorRowNum = Ref.shape[0]
	
	range_xlength = range(xlength)
	range_ylength = range_xlength
	range_inputVectorRowNum = range(inputVectorRowNum)
	range_refVectorRowNum = range(refVectorRowNum)
	
	#learning-rate coefficient
	eta = init_eta
	
	#average error
	averageError = zeros(learningTime)
	
	#View first refference vector space(Temporaly cord)
	coordinate = inputVector
	for i in range_inputVectorRowNum :
		Temp = ()
		for j in range_refVectorRowNum:
			Temp = Temp + tuple([norm(inputVector[i, :] - Ref[j, :])])
		minIndex = Temp.index(min(Temp))
		if Dimention == 1:
			coordinate[i] = minIndex
		else:
			coordinate[i, :] = array([int(fix(minIndex / xlength)), minIndex - int(fix(minIndex / xlength))*ylength])
	f = open("0epoch's_ref_coordinate.dat", 'w')
	io.write_array(f, coordinate, ' ')
	f.close()
	
	#learning
	print 'Learning Time :'
	for t in range(learningTime):
		if(((t + 1) % 1000) == 0):
			print (t+1)
		
		sumError = 0
		check = sample(xrange(inputVectorRowNum), inputVectorRowNum)
		#check = range(inputVectorRowNum)
		for cnt in range_inputVectorRowNum:
			#Choose an index of variable has minimum distance
			#Temp = zeros(inputVectorRowNum)
			Temp = ()
			for i in range_refVectorRowNum:
				Temp = Temp + tuple([norm(inputVector[check[cnt], :] - Ref[i, :])])
			
			#Winner-Takes-All(WTA)
			minIndex = Temp.index(min(Temp))
			
			tmp = 2 * (sigma**2)
			if(Dimention == 1):
				for x in range_refVectorRowNum:
					#neighbor function
					h = eta * exp(-(norm(minIndex - x)**2) / tmp)
					
					#renewal
					Ref[x,:] = Ref[x,:] + h*(inputVector[check[cnt], :] - Ref[x, :])
			else:
				Rc = array([int(fix(minIndex / xlength)), minIndex - int(fix(minIndex / xlength))*ylength])
				for y in range_ylength:
					for x in range_xlength:
						#neighbor function
						h = eta * exp(-(norm(Rc - array([x, y])))**2 /tmp)
						
						#renewal
						Ref[x + ylength * y,:] = Ref[x + ylength * y,:] + h*(inputVector[check[cnt], :] - Ref[x + ylength * y, :])
						
			#add a value of error
			sumError = sumError + norm(inputVector[check[cnt], :] - Ref[minIndex, :])
		
		#renewal eta
		#eta = init_eta * (1.0 - double(t + 1)/learningTime)
		eta = init_eta * exp(-2*init_eta * double(t + 1)/learningTime)

		#renewal sigma
		sigma = init_sigma * exp(-2*init_sigma * double(t + 1)/learningTime)

		#error at this epoch
		averageError[t] = sumError / refVectorRowNum
		
		#View Refference Vector Space
		if ((t+1) == (learningTime/2)) | ((t+1) == learningTime):
			for i in range_inputVectorRowNum :
				Temp = ()
				for j in range_refVectorRowNum:
					Temp = Temp + tuple([norm(inputVector[i, :] - Ref[j, :])])
				minIndex = Temp.index(min(Temp))
				if Dimention == 1:
					coordinate[i] = minIndex
				else:
					coordinate[i, :] = array([int(fix(minIndex / xlength)), minIndex - int(fix(minIndex / xlength))*ylength])
			f = open(str(t+1) + "epoch's_ref_coordinate.dat", 'w')
			io.write_array(f, coordinate, ' ')
			f.close()

	return Ref, averageError

#A execution of Python is accelerated by "Psyco".
psyco.full()

#loop number
try:
	loop_num = int(sys.argv[1])
except:
	print 'Exception : Please input ">python som.py [integer]"'
	exit()

#X = array([[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]]).T
#X = array([[1,1,1,2,2,2,3,3,3],[1,2,3,1,2,3,1,2,3]]).T
x = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
     2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
     3, 3, 3, 3, 3, 3, 3, 3, 3, 3, \
     4, 4, 4, 4, 4, 4, 4, 4, 4, 4, \
     5, 5, 5, 5, 5, 5, 5, 5, 5, 5, \
     6, 6, 6, 6, 6, 6, 6, 6, 6, 6, \
     7, 7, 7, 7, 7, 7, 7, 7, 7,7, \
     8, 8, 8, 8, 8, 8, 8, 8, 8, 8, \
     9, 9, 9, 9, 9, 9, 9, 9, 9, 9, \
     10,10,10,10,10,10,10,10,10,10]
y = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
     10, 9, 8, 7, 6, 5, 4, 3, 2, 1, \
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
     10, 9, 8, 7, 6, 5, 4, 3, 2, 1, \
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
     10, 9, 8, 7, 6, 5, 4, 3, 2, 1, \
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
     10, 9, 8, 7, 6, 5, 4, 3, 2, 1, \
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
     10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

X = array([x,y]).T
d = 2
t = 10000
eta = 0.9
list_err = []

for i in range(loop_num):
	[Ref, Err] = som(X, d, t, eta)
	
	f = open('som_ref' + str(i) + '.dat', 'w')
	io.write_array(f, Ref, ' ')
	f.close()
	
	list_err = list_err + [Err]
	
f = open('som_err_avrg.dat', 'w')
for i in range(t):
	sum = 0.0
	for j in range(loop_num):
		sum = sum + double(list_err[j][i])
	f.write(str(sum / loop_num)+'\n')

f.close()