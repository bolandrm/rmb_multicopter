import serial
from PySide.QtCore import QThread, Signal
from .serial_reader import SerialReader
from .serial_writer import SerialWriter

class SerialManager():
    SERIAL_PORT = "/dev/tty.usbmodem747851"

    def __init__(self):
        self.serialPort = serial.Serial(self.SERIAL_PORT, 115200, timeout=0.5)

        self.reader = SerialReader(self.serialPort)
        self.reader.start()

        self.writer = SerialWriter(self.serialPort)

    def stop(self):
        print("Cleaning up serial reader thread")

        while self.reader.isRunning():
            self.reader.finished()

        self.serialPort.close()

