from PySide.QtCore import QThread, Signal
import struct
import comms
import serial
import time

class SerialReader(QThread):
    is_finished = False
    on_config_received = Signal(object)
    on_gyro_acc_received = Signal(object)
    on_success = Signal()
    on_failure = Signal()

    def __init__(self, serial_port):
        self.serial_port = serial_port
        super(SerialReader, self).__init__()
        self.start()

    def run(self):
        code = 0
        state = 0
        crc = 0
        data_expected_length = 0
        data_received_length = 0
        data_buffer = bytes()
        data = 0

        while self.is_finished == False:
            if comms.SerialManager().port_is_open():
                try:
                    data = self.serial_port.read()
                except serial.serialutil.SerialException:
                    print("error - closing serial port")
                    comms.SerialManager().close_port()
            else:
                time.sleep(0.2)
                continue

            if data == b'':
                # this is for when the serial port times out.
                # does this have a chance of breaking things if the data is ever empty?
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
                    elif (code == comms.INFO_SUCCESS):
                        self.on_success.emit()
                    elif (code == comms.INFO_FAILURE):
                        self.on_failure.emit()
                    else:
                        self.log("unrecognized code: {}".format(code), True)
                else:
                    self.log("calc crc: {}".format(crc))
                    self.log("got crc: {}".format(ord(data)))
                    self.log("crc bad!")
                state = 0
                crc = 0
                data_expected_length = 0
                data_received_length = 0
                data_buffer = bytes()

    def unpack_gyro_acc_data(self, data_buffer):
        self.log("unpacking gyro acc data")
        data = struct.unpack("< fff fff fff hhh hhh fff fff", data_buffer)
        self.log("recieved gyro acc data: {}".format(data))
        self.on_gyro_acc_received.emit(data)

    def unpack_config_data(self, data_buffer):
        self.log("recieved config data")
        self.on_config_received.emit(data_buffer)

    def finished(self):
        self.is_finished = True

    def log(self, text, important=False):
        if not important: return
        print("[SerialReader] {}".format(text))
