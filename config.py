# -*- coding: utf-8 -*-

#import pyqtgraph as pg

import serial
import threading
import struct
import sys

# import numpy as np
# import PySide
# from PySide import QtGui

SERIAL_PORT = "/dev/tty.usbmodem747851"

# class Example(QtGui.QWidget):
#     def __init__(self):
#         super(Example, self).__init__()
#         self.initUI()
# 
#     def initUI(self):
#         btn = QtGui.QPushButton('Button', self)
#         btn.resize(btn.sizeHint())
#         btn.move(50, 50)
# 
#         self.setGeometry(300, 300, 250, 150)
#         self.setWindowTitle('Tooltips')
#         self.show()

def print_input(serial):
    while True:
        print(serial.readline())

def send_packet(serial_port, code, data):
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


def main():
#    app = QtGui.QApplication(sys.argv)
#    ex = Example()
#    sys.exit(app.exec_())
    serial_port = serial.Serial(SERIAL_PORT, 115200)
    thread = threading.Thread(target=print_input, args=(serial_port,))
    thread.start()

    config = (
                1,
                1.01,
                1.02,
                1.03,
                1.04,
                2.01,
                2.02,
                2.03,
                2.04,
                3.01,
                3.02,
                3.03,
                3.04,
                4.01,
                4.02,
                4.03,
                4.04,
                5.01,
                5.02,
                5.03,
                5.04,
                6.01,
                6.02,
                6.03,
                6.04,
            )
    send_packet(serial_port, 101, struct.pack("< h ffff ffff ffff ffff ffff ffff", *config))

if __name__ == '__main__':
    main()
