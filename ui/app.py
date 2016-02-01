#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import sys
import math
import numpy as np
from PySide import QtCore, QtGui, QtOpenGL
import PySide
import pyqtgraph as pg

from widgets import *
from comms import SerialManager

pg.setConfigOption('background', 'w')
pg.setConfigOption('foreground', 'k')

#class InfoWindow(QtGui.QWidget):
#    def __init__(self):
#        super().__init__()
#        self.setGeometry(703, 100, 500, 600)
#        self.setWindowTitle("Configuration")
#        layout = QtGui.QHBoxLayout()
#
#        self.orientation_widget = OrientationWidget()
#        layout.addWidget(self.orientation_widget)
#
#        self.setLayout(layout)
#
#    def show(self):
#        super().show()
#        self.orientation_widget.setupInitialRotation()

class InfoWindow(QtGui.QWidget):
    def __init__(self):
        super().__init__()
        self.setGeometry(703, 100, 800, 400)
        self.setWindowTitle("Configuration")
        layout = QtGui.QHBoxLayout()

        self.chart_widget = ChartWidget()
        layout.addWidget(self.chart_widget)

        self.setLayout(layout)

class ConfigWindow(QtGui.QMainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self.setGeometry(100, 100, 600, 700)
        self.setWindowTitle("Configuration")

        self.central_widget = ConfigWidget()
        self.setCentralWidget(self.central_widget)

        SerialManager().reader.on_success.connect(self.success_reported)
        SerialManager().reader.on_config_received.connect(self.config_loaded)

        self.statusBar().setStyleSheet("border-top: 1px dashed #666")
        self.statusBar().setSizeGripEnabled(False)

    def success_reported(self):
        self.statusBar().showMessage("config saved!", 2000)

    def config_loaded(self):
        self.statusBar().showMessage("loading config...", 1000)

def exit_handler():
    SerialManager().stop()

def main():
    app = QtGui.QApplication(sys.argv)
    app.aboutToQuit.connect(exit_handler)

    config_window = ConfigWindow()
    config_window.show()

    infoWindow = InfoWindow()
    infoWindow.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
