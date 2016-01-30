from PySide.QtCore import QThread, Signal
import struct

class SerialReader(QThread):
    PACKET_HEADER1 = 0x02
    PACKET_HEADER2 = 0xB5

    REQUEST_CONFIG = 0x01
    REQUEST_GYRO_ACC = 0x02

    is_finished = False
    config_received = Signal(object)

    def __init__(self, serial_port):
        self.serial_port = serial_port
        super(SerialReader, self).__init__()

    def run(self):
        code = 0
        state = 0
        crc = 0
        data_expected_length = 0
        data_received_length = 0
        data_buffer = bytes()

        while self.is_finished == False:
            data = self.serial_port.read()

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
                        self.unpack_config_data(data_buffer)
                    if (code == self.REQUEST_GYRO_ACC):
                        self.unpack_gyro_acc_data(data_buffer)
                else:
                    print("calc crc: {}".format(bytes([crc])))
                    print("got crc: {}".format(data))
                    print("crc bad!")
                state = 0
                crc = 0
                data_expected_length = 0
                data_received_length = 0
                data_buffer = bytes()

    def unpack_gyro_acc_data(self, data_buffer):
        print("unpacking gyro acc data")
        data = struct.unpack("< ffffff", data_buffer)
        print("recieved gyro acc data: {}".format(data))

    def unpack_config_data(self, data_buffer):
        print("unpacking config data")
        data = struct.unpack("< Hffffffff", data_buffer)
        self.config_received.emit(data)
        print("recieved config data: {}".format(data))

    def finished(self):
        self.is_finished = True

#config = (
#            1,
#            1.01,
#            1.02,
#            1.03,
#            1.04,
#        )
##send_packet(serial_port, 101, struct.pack("< h ffff ffff ffff ffff ffff ffff", *config))
