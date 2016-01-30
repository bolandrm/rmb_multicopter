# -*- coding: utf-8 -*-

#import pyqtgraph as pg

import sys
import time
import math
import numpy as np
from PySide import QtCore, QtGui, QtOpenGL

from widgets import *
from communication import SerialManager

def update_imu_data(imu_data):
    (imu_rate_x, imu_rate_y, imu_rate_z,
    imu_angles_x, imu_angles_y, imu_angles_z) = imu_data

    window.glWidget.setXRotation(-imu_angles_y)
    window.glWidget.setYRotation(imu_angles_x)

    print("imu data: {}", imu_data)


class InfoWindow(QtGui.QWidget):
    def __init__(self):
        super().__init__()
        self.setGeometry(703, 100, 500, 600)
        self.setWindowTitle("Configuration")
        self.orientationWidget = OrientationWidget()
        layout = QtGui.QHBoxLayout()
        layout.addWidget(self.orientationWidget)
        self.setLayout(layout)

    def show(self):
        super().show()
        self.orientationWidget.setXRotation(-10)
        self.orientationWidget.setYRotation(30)
        self.orientationWidget.setZRotation(0)

class ConfigWindow(QtGui.QMainWindow):
    def __init__(self, serialManager):
        QtGui.QMainWindow.__init__(self)
        self.setGeometry(100, 100, 600, 700)
        self.setWindowTitle("Configuration")

        self.centralWidget = ConfigWidget(serialManager)
        self.setCentralWidget(self.centralWidget)

def exitHandler(serialManager):
    serialManager.stop()

def main():
    app = QtGui.QApplication(sys.argv)

    serialManager = SerialManager()

    app.aboutToQuit.connect(lambda: exitHandler(serialManager))

    configWindow = ConfigWindow(serialManager)
    configWindow.show()

    #infoWindow = InfoWindow()
    #infoWindow.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
