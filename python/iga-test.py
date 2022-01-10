# -*- coding: utf-8 -*-
import random
import sys
from PyGa import *
from PyQt4 import QtGui, QtCore

#--------------------------------------------------
class UserInterface(QtGui.QWidget):    
    def __init__(self, parent=None):
	self.__row_num = 4
	self.__row_size = 90
	self.__column_num = 4
	self.__column_size = 120
	self.__QColorPairList = []
	self.__RGBPairList = []
	self.__QCheckBoxList = []

	#self is "class Interface"
	#   self   : Main Window
	#   WidgetLayout : GridLayout(Main Window)
	QtGui.QWidget.__init__(self, parent)
	WidgetLayout = QtGui.QGridLayout(self)

	self.setFixedSize(self.__row_num * self.__row_size, self.__column_num * self.__column_size)
	self.setWindowTitle('interactive Genetic Algorithm')
	WidgetLayout.setAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignBottom )

	for i in range(self.__row_num * (self.__column_num - 1)):
	    red = [random.randint(0,255), random.randint(0,255)]
	    blue = [random.randint(0,255), random.randint(0,255)]
	    green = [random.randint(0,255), random.randint(0,255)]

	    self.__QColorPairList.append([QtGui.QColor(red[0],blue[0],green[0]), QtGui.QColor(red[1],blue[1],green[1])])
	    self.__RGBPairList.append([red[0],blue[0],green[0],red[1],blue[1],green[1]])
	    
	    self.__QCheckBoxList.append(QtGui.QCheckBox("I Like it."))
	    self.__QCheckBoxList[i].setObjectName('check_box' + str(i))
	    self.__QCheckBoxList[i].setFixedWidth(self.__row_size)
	    self.__QCheckBoxList[i].setFixedHeight(self.__column_size)
	    WidgetLayout.addWidget(self.__QCheckBoxList[i], (i / self.__column_num), (i % self.__row_num))

	self.__Core = IGA(self.__RGBPairList, 8)
	crossover = QtGui.QPushButton('Crossover')
	quit = QtGui.QPushButton('Quit')
	crossover.setFixedWidth(self.__row_size)
	quit.setFixedWidth(self.__row_size)
	crossover.setObjectName('crossover_button')
	quit.setObjectName('quit_button')
	WidgetLayout.addWidget(crossover, 3, 2)
	WidgetLayout.addWidget(quit, 3, 3)
	
	QtCore.QMetaObject.connectSlotsByName(self)
	#self.connect(crossover, QtCore.SIGNAL('clicked()'), QtGui.qApp, QtCore.SLOT('quit()'))
	#self.connect(quit, QtCore.SIGNAL('clicked()'), QtGui.qApp, QtCore.SLOT('quit()'))

    def paintEvent(self, event):
	QPainter = QtGui.QPainter()
	WidthOffset = 30
	HeightOffset = 55
	RectWidth = 15
	RectHeight = 30

	if QPainter.begin(self):
	    for i in range(self.__row_num * (self.__column_num - 1)):
		QPainter.fillRect((self.__row_size*(i%self.__row_num)+WidthOffset), (self.__column_size*(i/self.__column_num)+HeightOffset), \
				  RectWidth, RectHeight, self.__QColorPairList[i][0])
		QPainter.fillRect((self.__row_size*(i%self.__row_num)+WidthOffset+RectWidth), (self.__column_size*(i/self.__column_num)+HeightOffset), \
				  RectWidth, RectHeight, self.__QColorPairList[i][1])
	if QPainter.end() == False:
	    self.close()

    @QtCore.pyqtSlot()
    def on_quit_button_clicked(self):
	print 'Signal : Quit'
	quit()
	
    @QtCore.pyqtSlot()
    def on_crossover_button_clicked(self):
	ChooseIndexList = []

	for i in xrange(len(self.__QCheckBoxList)):
	    if self.__QCheckBoxList[i].isChecked() is True:
		ChooseIndexList.append(i)
		self.__QCheckBoxList[i].toggle()
	    
	if len(ChooseIndexList) == 0:
	    print 'Signal : Initialize'
	    self.__QColorPairList = []
	    self.__RGBPairList = []
	    for i in range(self.__row_num * (self.__column_num - 1)):
		red = [random.randint(0,255), random.randint(0,255)]
		blue = [random.randint(0,255), random.randint(0,255)]
		green = [random.randint(0,255), random.randint(0,255)]
		
		self.__QColorPairList.append([QtGui.QColor(red[0],blue[0],green[0]), QtGui.QColor(red[1],blue[1],green[1])])
		self.__RGBPairList.append([red[0],blue[0],green[0],red[1],blue[1],green[1]])
	else:
	    print 'Signal : Crossover'
	    self.__Core.CrossOver(ChooseIndexList)
	    work = self.__Core.getGeneList()

	    print work, len(work)
	    for i in xrange(len(work)):
		print i, work[i].getData()
		for j in xrange(len(work[i].getData())):
		    self.__RGBPairList[i][j] = int(work[i].getData()[j], 2)
		self.__QColorPairList[i] = [QtGui.QColor(self.__RGBPairList[i][0],self.__RGBPairList[i][1],self.__RGBPairList[i][2]), \
					    QtGui.QColor(self.__RGBPairList[i][3],self.__RGBPairList[i][4],self.__RGBPairList[i][5])]
	self.update()
#--------------------------------------------------
	
app = QtGui.QApplication(sys.argv)
UI = UserInterface()
UI.show()
sys.exit(app.exec_())
