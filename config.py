# -*- coding: utf-8 -*-

#import pyqtgraph as pg

import serial
import threading
import struct
import sys
import time
import math
import numpy as np
from PySide import QtCore, QtGui, QtOpenGL
from OpenGL import GL

# import PySide
# from PySide import QtGui

SERIAL_PORT = "/dev/tty.usbmodem747851"
PACKET_HEADER1 = b"\x02"
PACKET_HEADER2 = b"\xB5"

window = None

def print_input(serial):
    code = 0
    state = 0
    crc = 0
    data_expected_length = 0
    data_received_length = 0
    data_buffer = bytes()

    while True:
        data = serial.read()

        if (state == 0):
            if (data == PACKET_HEADER1):
                state += 1
                print("got header1")
        elif (state == 1):
            if (data == PACKET_HEADER2):
                state += 1
                print("got header2")
            else:
                state = 0
        elif (state == 2):
            code = data
            crc ^= ord(data)
            state += 1
        elif (state == 3):
            data_expected_length = ord(data)
            crc ^= ord(data)
            state += 1
        elif (state == 4):
            data_expected_length |= (ord(data) << 8)
            crc ^= ord(data)
            state += 1
            print("data length: {}".format(data_expected_length))
        elif (state == 5):
            data_buffer += data
            crc ^= ord(data)
            data_received_length += 1
            if (data_received_length >= data_expected_length):
                state += 1
        elif (state == 6):
            if (bytes([crc]) == data):
                print("crc correct!")
                update_imu_data(struct.unpack("< ffffff", data_buffer))
            else:
                print("calc crc: {}".format(bytes([crc])))
                print("got crc: {}".format(data))
                print("crc bad!")
            state = 0
            crc = 0
            data_expected_length = 0
            data_received_length = 0
            data_buffer = bytes()

def update_imu_data(imu_data):
    global window

    (imu_rate_x, imu_rate_y, imu_rate_z,
    imu_angles_x, imu_angles_y, imu_angles_z) = imu_data

    window.glWidget.setXRotation(-imu_angles_y)
    window.glWidget.setYRotation(imu_angles_x)

    print("imu data: {}", imu_data)

def send_packet(serial_port, code, data=None):
    if data == None: data = struct.pack("< B", 0x0)
    size = len(data)
    print("size is {}".format(size))

    packet_body = bytes()
    packet_body += struct.pack("< Bh", code, size)
    packet_body += data

    crc = 0x00
    for b in packet_body:
        crc = crc ^ b

    packet = bytes()
    packet += struct.pack("< BB", 0x02, 0xB5)
    packet += packet_body
    packet += struct.pack("< B", crc)

    serial_port.write(packet)
    print(packet)

def request_gyro_acc(serial_port):
    while True:
        time.sleep(0.1)
        send_packet(serial_port, 2)


class Window(QtGui.QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.glWidget = GLWidget()

        mainLayout = QtGui.QHBoxLayout()
        mainLayout.addWidget(self.glWidget)
        self.setLayout(mainLayout)

        self.setWindowTitle(self.tr("openGL"))

    def show(self):
        super().show()

        self.glWidget.setXRotation(-10)
        self.glWidget.setYRotation(30)
        self.glWidget.setZRotation(0)

class GLWidget(QtOpenGL.QGLWidget):
    def __init__(self, parent=None):
        QtOpenGL.QGLWidget.__init__(self, parent)

        print("widget init")
        self.object = 0
        self.xRot = 0
        self.yRot = 0
        self.zRot = 0

        self.colorBlue = QtGui.QColor.fromRgb(0, 0, 240)
        self.colorGray = QtGui.QColor.fromRgb(177, 177, 177)
        self.colorGreen = QtGui.QColor.fromRgb(0, 240, 0)
        self.colorWhite = QtGui.QColor.fromRgb(255, 255, 255)

    def xRotation(self):
        return self.xRot

    def yRotation(self):
        return self.yRot

    def zRotation(self):
        return self.zRot

    def minimumSizeHint(self):
        return QtCore.QSize(50, 50)

    def sizeHint(self):
        return QtCore.QSize(400, 400)

    def setXRotation(self, angle):
        angle = (angle + 90) * 16
        if angle != self.xRot:
            self.xRot = angle
            self.updateGL()

    def setYRotation(self, angle):
        angle = angle * 16
        if angle != self.yRot:
            self.yRot = angle
            self.updateGL()

    def setZRotation(self, angle):
        angle = (angle + 135) * 16
        if angle != self.zRot:
            self.zRot = angle
            self.updateGL()

    def initializeGL(self):
        self.qglClearColor(self.colorWhite)
        self.object = self.makeObject()
        GL.glShadeModel(GL.GL_FLAT)
        GL.glEnable(GL.GL_DEPTH_TEST)
        GL.glEnable(GL.GL_CULL_FACE)

    def paintGL(self):
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
        GL.glLoadIdentity()
        GL.glTranslated(0.0, 0.0, -10.0)
        GL.glRotated(self.xRot / 16.0, 1.0, 0.0, 0.0)
        GL.glRotated(self.yRot / 16.0, 0.0, 1.0, 0.0)
        GL.glRotated(self.zRot / 16.0, 0.0, 0.0, 1.0)
        GL.glCallList(self.object)

    def resizeGL(self, width, height):
        side = min(width, height)
        GL.glViewport(int((width - side) / 2), int((height - side) / 2), side, side)

        GL.glMatrixMode(GL.GL_PROJECTION)
        GL.glLoadIdentity()
        GL.glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0)
        GL.glMatrixMode(GL.GL_MODELVIEW)

    def makeObject(self):
        genList = GL.glGenLists(1)
        GL.glNewList(genList, GL.GL_COMPILE)

        GL.glBegin(GL.GL_QUADS)

        x1 = +0.35
        y1 = -0.10
        x2 = +0.10
        y2 = -0.35

        self.qglColor(self.colorBlue)
        self.quad(x1, y1, x2, y2, y2, x2, y1, x1)

        self.qglColor(self.colorGreen)
        self.extrude(x1, y1, x2, y2)
        self.extrude(y2, x2, y1, x1)

        self.qglColor(self.colorGray)
        self.extrude(x2, y2, y2, x2)
        self.extrude(y1, x1, x1, y1)

        GL.glEnd()
        GL.glEndList()

        return genList

    def quad(self, x1, y1, x2, y2, x3, y3, x4, y4):
        GL.glVertex3d(x1, y1, +0.07)
        GL.glVertex3d(x2, y2, +0.07)
        GL.glVertex3d(x3, y3, +0.07)
        GL.glVertex3d(x4, y4, +0.07)

        GL.glVertex3d(x4, y4, -0.07)
        GL.glVertex3d(x3, y3, -0.07)
        GL.glVertex3d(x2, y2, -0.07)
        GL.glVertex3d(x1, y1, -0.07)

    def extrude(self, x1, y1, x2, y2):
        GL.glVertex3d(x1, y1, -0.07)
        GL.glVertex3d(x2, y2, -0.07)
        GL.glVertex3d(x2, y2, +0.07)
        GL.glVertex3d(x1, y1, +0.07)

def main():
    global window
#    app = QtGui.QApplication(sys.argv)
#    ex = Example()
#    sys.exit(app.exec_())

    app = QtGui.QApplication(sys.argv)
    window = Window()
    window.show()

    serial_port = serial.Serial(SERIAL_PORT, 115200)
    thread1 = threading.Thread(target=print_input, args=(serial_port,))
    thread1.start()

    thread2 = threading.Thread(target=request_gyro_acc, args=(serial_port,))
    thread2.start()

    sys.exit(app.exec_())

    #config = (
    #            1,
    #            1.01,
    #            1.02,
    #            1.03,
    #            1.04,
    #            2.01,
    #            2.02,
    #            2.03,
    #            2.04,
    #            3.01,
    #            3.02,
    #            3.03,
    #            3.04,
    #            4.01,
    #            4.02,
    #            4.03,
    #            4.04,
    #            5.01,
    #            5.02,
    #            5.03,
    #            5.04,
    #            6.01,
    #            6.02,
    #            6.03,
    #            6.04,
    #        )
    ##send_packet(serial_port, 101, struct.pack("< h ffff ffff ffff ffff ffff ffff", *config))

if __name__ == '__main__':
    main()
