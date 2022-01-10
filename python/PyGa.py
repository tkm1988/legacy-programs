# -*- coding: utf-8 -*-
from types import *
from math import *
import random

class SimpleGene:
    def __init__(self, Input = [], strlen = 0):
	self.__data = []
	if len(Input) >= 1:
	    if type(Input) is ListType:
		if type(Input[0]) is IntType:
		    for i in xrange(len(Input)):
			self.__data.append(dec2bin(Input[i], strlen))
		else:
		    raise TypeError, 'Attributes in list for SimpleGene can use only IntType.'
	    else:
		raise TypeError, 'Input for SimpleGene must be ListType.'

    def getData(self):
	return self.__data
    
    def setData(self, data):
	self.__data, data = data, self.__data

    def OnePointCrossover(self, gene):
	result = []
	tmp_result1 = SimpleGene()
	tmp_result2 = SimpleGene()
	
	if len(self.__data) == len(gene.getData()):
	    for i in xrange(len(self.__data)):
		point = random.randint(0, len(self.__data[i]))

		#CROSSOVER
		if len(self.__data[i]) == len(gene.getData()[i]):
		    tmp_result1.setData(self.__data[i][0:point] + gene.getData()[i][point:])
		    tmp_result2.setData(gene.getData()[i][0:point]      + self.__data[i][point:])
		else:
		    raise AttributeError, 'len(self.__data['+str(i)+']) not equal len(gene['+str(i)+'])'
	    result.append(tmp_result1)
	    result.append(tmp_result2)
	else:
	    raise AttributeError, 'len(self.__data) not equal len(gene)'
	return result

    def TwoPointCrossover(self, gene):
	result = []
	tmp_list1 = []
	tmp_list2 = []
	tmp_result1 = SimpleGene()
	tmp_result2 = SimpleGene()

	if len(self.__data) == len(gene.getData()):
	    for i in xrange(len(self.__data)):
		point1 = random.randint(0, len(self.__data[i])/2)
		point2 = random.randint(point1, len(self.__data[i]))
		#CROSSOVER
		if len(self.__data[i]) == len(gene.getData()[i]):
		    tmp_list1.append(self.__data[i][0:point1] + gene.getData()[i][point1:point2]      + self.__data[i][point2:])
		    tmp_list2.append(gene.getData()[i][0:point1]      + self.__data[i][point1:point2] + gene.getData()[i][point2:])
		else:
		    raise AttributeError, 'len(self.__data['+str(i)+']) not equal len(gene['+str(i)+'])'
	    tmp_result1.setData(tmp_list1)
	    tmp_result2.setData(tmp_list2)
	    result.append(tmp_result1)
	    result.append(tmp_result2)
	else:
	    raise AttributeError, 'len(self.__data) not equal len(gene)'
	return result
    
    def Mutation(self):
	index = random.randint(0, len(self.__data))
	mutation_point = random.randint(0, len(self.__data[index]))
	self.__data[index][mutation_point] = '1' if self.__data[index][mutation_point] is '0' else '0' 

class IGA:
    __terminate = 0
    #GeneList = [SimpleGene0, SimpleGene1, ... ]
    def __init__(self, BaseData, bit_num):
	self.__GeneList = []
	if type(BaseData) is ListType:
            #本IGAでは3個未満の個体数のIGAは認めない
	    if len(BaseData) > 3:
		for i in xrange(len(BaseData)):
		    try:
			self.__GeneList.append(SimpleGene(BaseData[i], bit_num))
		    except TypeError, detail:
			print detail
	    else:
		raise IndexError,'Length of BaseData is bigger than 3.'
	else:
	    raise TypeError, 'BaseData must be ListType.'
	self.__terminate = len(self.__GeneList) / 2
	
    def CrossOver(self, ChooseIndexList):
	if type(ChooseIndexList) is ListType:
	    if len(ChooseIndexList) is not 0:
		odd_flag = False
		swaplist = []
		ChooseIndexList_Length = len(ChooseIndexList)
		print 'ChooseIndexList_Length = ', ChooseIndexList_Length
		
		#親個体をリストに格納
		for i in xrange(ChooseIndexList_Length):
		    if len(swaplist) < self.__terminate:
			swaplist.append(self.__GeneList[ChooseIndexList[i]])
		print 'SwapList size = ', len(swaplist), ', Terminate is ',self.__terminate
		
		#母集団数の半分に満たない場合は，重心からのユークリッド距離の近い親個体を選ぶ
		if ChooseIndexList_Length < self.__terminate:
		    print 'Signal : Machine Choice[', self.__terminate - ChooseIndexList_Length,']'
		    CenterOfGravity = [] # is mathmatical vector
		    #Ranking = [[GeneList's Index, dist], ... ]
		    Ranking = []

		    for i in xrange(len(swaplist[0].getData())):
			tmp = 0.0
			for j in xrange(len(swaplist)):
			    tmp = tmp + int(swaplist[j].getData()[i], 2)
			CenterOfGravity.append(tmp/len(swaplist[0].getData()))
		    
		    for i in xrange(len(self.__GeneList)):
			if i not in ChooseIndexList:
			    tmp = 0.0

			    for j in xrange(len(self.__GeneList[i].getData())):
				tmp = tmp + (CenterOfGravity[j] - int(self.__GeneList[i].getData()[j], 2))**2
			    Ranking.append([i, sqrt(tmp)])

		    Ranking.sort(cmp = lambda x, y:cmp(x[1], y[1]))

		    for i in xrange(self.__terminate - ChooseIndexList_Length):
			swaplist.append(self.__GeneList[Ranking[i][0]])
			ChooseIndexList.append(Ranking[i][0])

		#二点交叉により残り半分の次世代の個体を作る
		if (len(self.__GeneList) % 2) is 1:
		    odd_flag = True
		ChooseIndexList = random.sample(ChooseIndexList, self.__terminate)

		for i in xrange(0, self.__terminate, 2):
		    templist = self.__GeneList[ChooseIndexList[i]].TwoPointCrossover(self.__GeneList[ChooseIndexList[i+1]])
		    swaplist.append(templist[0])
		    swaplist.append(templist[1])
	        #奇数の場合は個体数が1個減少してしまうので，ランダムに親を決めて，子どもの1つを残す
		if odd_flag is True :
		    templist = self.__GeneList[ChooseIndex[random.randint(0,self.__terminate)]].TwoPointCrossover(\
			self.__GeneList[ChooseIndex[random.randint(0,self.__terminate)]])
		    swaplist.append(templist[random.randint(0,1)])

		self.__GeneList, swaplist = swaplist, self.__GeneList
	    else:
		raise IndexError, 'Argument1\'s length must be longer than 0.'
	else:
	    raise TypeError, 'Argument1 must be ListType'

    def Mutation(self):
	random.shuffle(self.__GeneList[self.__tereminate : len(self.Genelist)])[0].Mutation()

    def getGeneList(self):
	return self.__GeneList

#    def Clustering(self):
	
def dec2bin(TargetNum, strlen = 0):
    result = ''
    ptr = '01'

    if type(strlen) is not IntType:
	raise TypeError, 'Argument2\'s type can use only IntType.'
												
    if type(TargetNum) is IntType:
	while TargetNum is not 0:
	    result = ptr[TargetNum % 2] + result
	    TargetNum = TargetNum / 2
    else:
	raise ValueError, 'TargetNum\'s type can use only IntType.'

    if strlen is not 0:
	if len(result) < strlen:
	    for i in xrange(strlen - len(result)):
		result = '0' + result
	else:
	    result = result[0:strlen]
	    
    return result
