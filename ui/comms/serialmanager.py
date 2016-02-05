import serial
from PySide.QtCore import QThread, Signal, QMutex
from libs.singleton import Singleton
import time
import comms

class SerialManager(metaclass=Singleton):
    allow_port_access = False

    def __init__(self):
        self.port_close_mutex = QMutex()
        self.serial_port = serial.Serial(baudrate=115200, timeout=0.2)

        self.reader = comms.SerialReader(self.serial_port)
        self.writer = comms.SerialWriter(self.serial_port)

    def stop(self):
        print("Cleaning up serial writer thread")
        while self.writer.isRunning():
            self.writer.finished()

        print("Cleaning up serial reader thread")
        while self.reader.isRunning():
            self.reader.finished()

        self.serial_port.close()

    def close_port(self):
        self.allow_port_access = False
        time.sleep(0.3)

        if self.serial_port.isOpen():
            self.serial_port.close()

    def open_port(self, port):
        print("opening port {}".format(port))
        self.serial_port.port = port
        self.serial_port.open()
        self.allow_port_access = True

    def port_is_open(self):
        return self.serial_port.isOpen() and self.allow_port_access

