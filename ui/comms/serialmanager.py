import serial
from PySide.QtCore import QThread, Signal
from libs.singleton import Singleton
import comms

class SerialManager(metaclass=Singleton):
    def __init__(self):
        self.serial_port = serial.Serial(comms.SERIAL_PORT, 115200, timeout=0.5)

        self.reader = comms.SerialReader(self.serial_port)
        self.reader.start()

        self.writer = comms.SerialWriter(self.serial_port)

    def stop(self):
        print("Cleaning up serial reader thread")

        while self.reader.isRunning():
            self.reader.finished()

        self.serial_port.close()

