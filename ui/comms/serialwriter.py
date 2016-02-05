from PySide.QtCore import QThread, Signal
import struct
import comms
import time
import serial

class DataRequestThread(QThread):
    is_finished = False

    def __init__(self):
        super(DataRequestThread, self).__init__()
        self.start()

    def run(self):
        while self.is_finished == False:
            comms.SerialManager().writer.send_packet(comms.REQUEST_GYRO_ACC)
            time.sleep(0.1)

class SerialWriter():
    def __init__(self, serial_port):
        self.serial_port = serial_port
        self.request_thread = DataRequestThread()

    def send_packet(self, code, data=None):
        if data == None: data = struct.pack("< x")
        size = len(data)

        packet_body = bytes()
        packet_body += struct.pack("< BH", code, size)
        packet_body += data

        crc = 0x00
        for b in packet_body:
            crc = crc ^ b

        packet = bytes()
        packet += struct.pack("< BB", comms.PACKET_HEADER1, comms.PACKET_HEADER2)
        packet += packet_body
        packet += struct.pack("< B", crc)

        if comms.SerialManager().port_is_open():
            try:
                self.serial_port.write(packet)
            except serial.serialutil.SerialException:
                print("error - closing serial port")
                comms.SerialManager().close_port()

    def finished(self):
        self.request_thread.is_finished = True

    def isRunning(self):
        return self.request_thread.isRunning()
