import struct
from .serial_reader import SerialReader

class SerialWriter():
    def __init__(self, serialPort):
        self.serialPort = serialPort

    def sendPacket(self, code, data=None):
        if data == None: data = struct.pack("< x")
        size = len(data)
        print("size is {}".format(size))

        packet_body = bytes()
        packet_body += struct.pack("< Bh", code, size)
        packet_body += data

        crc = 0x00
        for b in packet_body:
            crc = crc ^ b

        packet = bytes()
        packet += struct.pack("< BB", SerialReader.PACKET_HEADER1, SerialReader.PACKET_HEADER2)
        packet += packet_body
        packet += struct.pack("< B", crc)

        self.serialPort.write(packet)
        print(packet)

    #def request_gyro_acc(serialPort):
    #    while True:
    #        time.sleep(0.1)
    #        sendPacket(serialPort, 2)
