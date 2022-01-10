#!/usr/bin/python

# simple.py

import sys
#import psyco
from PyQt4 import QtGui

#psyco.full()
app = QtGui.QApplication(sys.argv)

widget = QtGui.QWidget()
widget.resize(250, 150)
widget.setWindowTitle('simple')
widget.show()

sys.exit(app.exec_())
