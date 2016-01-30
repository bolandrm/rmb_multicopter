from PySide.QtCore import QThread, Signal
import struct

class SerialReader(QThread):
    PACKET_HEADER1 = 0x02
    PACKET_HEADER2 = 0xB5

    REQUEST_CONFIG = 0x01
    REQUEST_GYRO_ACC = 0x02

    isFinished = False
    configReceived = Signal(object)

    def __init__(self, serialPort):
        self.serialPort = serialPort
        super(SerialReader, self).__init__()

    def run(self):
        code = 0
        state = 0
        crc = 0
        data_expected_length = 0
        data_received_length = 0
        data_buffer = bytes()

        while self.isFinished == False:
            data = self.serialPort.read()

            if data == b'':
                continue

            if (state == 0):
                if (ord(data) == self.PACKET_HEADER1):
                    state += 1
                    print("got header1")
            elif (state == 1):
                if (ord(data) == self.PACKET_HEADER2):
                    state += 1
                    print("got header2")
                else:
                    state = 0
            elif (state == 2):
                code = ord(data)
                print("code: {}".format(code))
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
                if (crc == ord(data)):
                    print("crc correct!")
                    print("expected: {}".format(data_received_length))
                    print("recieved: {}".format(data_expected_length))

                    if (code == self.REQUEST_CONFIG):
                        self.unpackConfigData(data_buffer)
                    if (code == self.REQUEST_GYRO_ACC):
                        self.unpackGyroAccData(data_buffer)
                else:
                    print("calc crc: {}".format(bytes([crc])))
                    print("got crc: {}".format(data))
                    print("crc bad!")
                state = 0
                crc = 0
                data_expected_length = 0
                data_received_length = 0
                data_buffer = bytes()

    def unpackGyroAccData(self, data_buffer):
        print("unpacking gyro acc data")
        data = struct.unpack("< ffffff", data_buffer)
        print("recieved gyro acc data: {}".format(data))

    def unpackConfigData(self, data_buffer):
        print("unpacking config data")
        data = struct.unpack("< Hffffffff", data_buffer)
        print("recieved config data: {}".format(data))

    def finished(self):
        self.isFinished = True

#config = (
#            1,
#            1.01,
#            1.02,
#            1.03,
#            1.04,
#        )
##send_packet(serial_port, 101, struct.pack("< h ffff ffff ffff ffff ffff ffff", *config))
