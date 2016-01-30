#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#import pyqtgraph as pg

import sys
import time
import math
import numpy as np
from PySide import QtCore, QtGui, QtOpenGL

from widgets import *
from comms import SerialManager

class InfoWindow(QtGui.QWidget):
    def __init__(self):
        super().__init__()
        self.setGeometry(703, 100, 500, 600)
        self.setWindowTitle("Configuration")
        self.orientation_widget = OrientationWidget()
        layout = QtGui.QHBoxLayout()
        layout.addWidget(self.orientation_widget)
        self.setLayout(layout)

    def show(self):
        super().show()
        self.orientation_widget.setXRotation(-10)
        self.orientation_widget.setYRotation(30)
        self.orientation_widget.setZRotation(0)

class ConfigWindow(QtGui.QMainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self.setGeometry(100, 100, 600, 700)
        self.setWindowTitle("Configuration")

        self.central_widget = ConfigWidget()
        self.setCentralWidget(self.central_widget)

def exit_handler():
    SerialManager().stop()

def main():
    app = QtGui.QApplication(sys.argv)
    app.aboutToQuit.connect(exit_handler)

    config_window = ConfigWindow()
    config_window.show()

    #infoWindow = InfoWindow()
    #infoWindow.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
