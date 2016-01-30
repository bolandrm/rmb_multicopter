from PySide import QtCore
from PySide.QtGui import *

from comms import SerialReader
from configsync import ConfigSync
from .pidconfigrow import PidConfigRow

class ConfigWidget(QWidget):
    def __init__(self, serial_manager):
        super().__init__()

        self.config = ConfigSync()

        self.serial_manager = serial_manager
        self.serial_manager.reader.config_received.connect(self.config_received)

        self.layout = QVBoxLayout()

        self.layout.addWidget(QLabel("Rate PID"))

        self.rate_pid_xy_row = PidConfigRow("X/Y", key="rate_xy", config=self.config)
        self.layout.addWidget(self.rate_pid_xy_row)

        self.rate_pid_z_row = PidConfigRow("Z", key="rate_z", config=self.config)
        self.layout.addWidget(self.rate_pid_z_row)

        self.layout.addWidget(QLabel("Angle PID"))

        self.angle_pid_xy_row = PidConfigRow("X/Y", key="angle_xy", config=self.config)
        self.layout.addWidget(self.angle_pid_xy_row)

        self.angle_pid_z_row = PidConfigRow("Z", key="angle_z", config=self.config)
        self.layout.addWidget(self.angle_pid_z_row)

        loadButton = QPushButton("Load Config")
        loadButton.clicked.connect(self.load_config)
        saveButton = QPushButton("Save Config")
        saveButton.clicked.connect(self.save_config)

        self.buttonLayout = QHBoxLayout()
        self.buttonLayout.addWidget(loadButton)
        self.buttonLayout.addWidget(saveButton)
        self.layout.addLayout(self.buttonLayout)

        self.layout.addStretch(1)
        self.setLayout(self.layout)

    def save_config(self):
        print("config saved")

    def load_config(self):
        print("config loaded")

        self.serial_manager.writer.send_packet(SerialReader.REQUEST_CONFIG)

    def config_received(self, data):
        print("widget got config data {}".format(data))
