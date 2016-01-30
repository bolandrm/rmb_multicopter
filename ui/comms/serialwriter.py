import struct
from .serialreader import SerialReader

class SerialWriter():
    def __init__(self, serial_port):
        self.serial_port = serial_port

    def send_packet(self, code, data=None):
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

        self.serial_port.write(packet)
        print(packet)

    #def request_gyro_acc(serial_port):
    #    while True:
    #        time.sleep(0.1)
    #        send_packet(serial_port, 2)
