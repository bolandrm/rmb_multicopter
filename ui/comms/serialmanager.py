import serial
from PySide.QtCore import QThread, Signal
from .serialreader import SerialReader
from .serialwriter import SerialWriter

class SerialManager():
    SERIAL_PORT = "/dev/tty.usbmodem747851"

    def __init__(self):
        self.serial_port = serial.Serial(self.SERIAL_PORT, 115200, timeout=0.5)

        self.reader = SerialReader(self.serial_port)
        self.reader.start()

        self.writer = SerialWriter(self.serial_port)

    def stop(self):
        print("Cleaning up serial reader thread")

        while self.reader.isRunning():
            self.reader.finished()

        self.serial_port.close()

