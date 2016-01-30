from PySide.QtCore import QThread, Signal
import struct
import comms

class SerialReader(QThread):
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
                if (ord(data) == comms.PACKET_HEADER1):
                    state += 1
                    self.log("got header1")
            elif (state == 1):
                if (ord(data) == comms.PACKET_HEADER2):
                    state += 1
                    self.log("got header2")
                else:
                    state = 0
            elif (state == 2):
                code = ord(data)
                self.log("code: {}".format(code))
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
                self.log("data length: {}".format(data_expected_length))
            elif (state == 5):
                data_buffer += data
                crc ^= ord(data)
                data_received_length += 1
                if (data_received_length >= data_expected_length):
                    state += 1
            elif (state == 6):
                self.log("expected: {}".format(data_expected_length))
                self.log("received: {}".format(data_received_length))

                if (crc == ord(data)):
                    self.log("crc correct!")

                    if (code == comms.REQUEST_CONFIG):
                        self.unpack_config_data(data_buffer)
                    elif (code == comms.REQUEST_GYRO_ACC):
                        self.unpack_gyro_acc_data(data_buffer)
                    else:
                        self.log("unrecognized code: {}".format(code))
                else:
                    self.log("calc crc: {}".format(bytes([crc])))
                    self.log("got crc: {}".format(data))
                    self.log("crc bad!")
                state = 0
                crc = 0
                data_expected_length = 0
                data_received_length = 0
                data_buffer = bytes()

    def unpack_gyro_acc_data(self, data_buffer):
        self.log("unpacking gyro acc data")
        data = struct.unpack("< ffffff", data_buffer)
        self.log("recieved gyro acc data: {}".format(data))

    def unpack_config_data(self, data_buffer):
        self.log("recieved config data")
        self.config_received.emit(data_buffer)

    def finished(self):
        self.is_finished = True

    def log(self, text):
        print("[SerialReader] {}".format(text))
