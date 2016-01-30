from PySide import QtCore
from PySide.QtGui import *
from comms import SerialReader
from libs.pyqtconfig import ConfigManager

class ConfigWidget(QWidget):
    def __init__(self, serial_manager):
        super().__init__()

        defaults = {
            "pid_rate_xy_kp": 0.0,
            "pid_rate_xy_pmax": 0.0,
            "pid_rate_xy_ki": 0.0,
            "pid_rate_xy_imax": 0.0,
            "pid_rate_z_kp": 0.0,
            "pid_rate_z_pmax": 0.0,
            "pid_rate_z_ki": 0.0,
            "pid_rate_z_imax": 0.0
        }

        self.config = ConfigManager()
        self.config.set_defaults(defaults)

        self.serial_manager = serial_manager
        self.serial_manager.reader.config_received.connect(self.config_received)

        self.layout = QGridLayout()

        self.layout.setColumnMinimumWidth(1, 15)
        self.layout.setColumnMinimumWidth(4, 15)
        self.layout.setColumnMinimumWidth(7, 15)
        self.layout.setColumnMinimumWidth(10, 15)

        self.rate_pid_inputs = {"XY": {}, "Z": {}}
        self.rate_pid_inputs["XY"]["P"] = QDoubleSpinBox()
        self.rate_pid_inputs["XY"]["P_MAX"] = QDoubleSpinBox()
        self.rate_pid_inputs["XY"]["I"] = QDoubleSpinBox()
        self.rate_pid_inputs["XY"]["I_MAX"] = QDoubleSpinBox()
        self.rate_pid_inputs["Z"]["P"] = QDoubleSpinBox()
        self.rate_pid_inputs["Z"]["P_MAX"] = QDoubleSpinBox()
        self.rate_pid_inputs["Z"]["I"] = QDoubleSpinBox()
        self.rate_pid_inputs["Z"]["I_MAX"] = QDoubleSpinBox()

        self.rate_pid_inputs["XY"]["P"].setDecimals(3)
        self.rate_pid_inputs["XY"]["P_MAX"].setDecimals(3)
        self.rate_pid_inputs["XY"]["I"].setDecimals(3)
        self.rate_pid_inputs["XY"]["I_MAX"].setDecimals(3)
        self.rate_pid_inputs["Z"]["P"].setDecimals(3)
        self.rate_pid_inputs["Z"]["P_MAX"].setDecimals(3)
        self.rate_pid_inputs["Z"]["I"].setDecimals(3)
        self.rate_pid_inputs["Z"]["I_MAX"].setDecimals(3)

        self.config.add_handler("pid_rate_xy_kp", self.rate_pid_inputs["XY"]["P"])
        self.config.add_handler("pid_rate_xy_pmax", self.rate_pid_inputs["XY"]["P_MAX"])
        self.config.add_handler("pid_rate_xy_ki", self.rate_pid_inputs["XY"]["I"])
        self.config.add_handler("pid_rate_xy_imax", self.rate_pid_inputs["XY"]["I_MAX"])
        self.config.add_handler("pid_rate_z_kp", self.rate_pid_inputs["XY"]["P"])
        self.config.add_handler("pid_rate_z_pmax", self.rate_pid_inputs["XY"]["P_MAX"])
        self.config.add_handler("pid_rate_z_ki", self.rate_pid_inputs["XY"]["I"])
        self.config.add_handler("pid_rate_z_imax", self.rate_pid_inputs["XY"]["I_MAX"])

        self.layout.addWidget(QLabel("Rate PID"), 0, 0)

        rate_pid_xy_row = 1

        self.layout.addWidget(QLabel("X/Y"), rate_pid_xy_row, 0)

        self.layout.addWidget(QLabel("P"), rate_pid_xy_row, 2)
        self.layout.addWidget(self.rate_pid_inputs["XY"]["P"], rate_pid_xy_row, 3)
        self.layout.addWidget(QLabel("P_MAX"), rate_pid_xy_row, 5)
        self.layout.addWidget(self.rate_pid_inputs["XY"]["P_MAX"],
                rate_pid_xy_row, 6)
        self.layout.addWidget(QLabel("I"), rate_pid_xy_row, 8)
        self.layout.addWidget(self.rate_pid_inputs["XY"]["I"], rate_pid_xy_row, 9)
        self.layout.addWidget(QLabel("I_MAX"), rate_pid_xy_row, 11)
        self.layout.addWidget(self.rate_pid_inputs["XY"]["I_MAX"],
                rate_pid_xy_row, 12)


        rate_pid_z_row = 2
        self.layout.addWidget(QLabel("Z"), rate_pid_z_row, 0)

        self.layout.addWidget(QLabel("P"), rate_pid_z_row, 2)
        self.layout.addWidget(self.rate_pid_inputs["Z"]["P"], rate_pid_z_row, 3)
        self.layout.addWidget(QLabel("P_MAX"), rate_pid_z_row, 5)
        self.layout.addWidget(self.rate_pid_inputs["Z"]["P_MAX"], rate_pid_z_row, 6)
        self.layout.addWidget(QLabel("I"), rate_pid_z_row, 8)
        self.layout.addWidget(self.rate_pid_inputs["Z"]["I"], rate_pid_z_row, 9)
        self.layout.addWidget(QLabel("I_MAX"), rate_pid_z_row, 11)
        self.layout.addWidget(self.rate_pid_inputs["Z"]["I_MAX"], rate_pid_z_row, 12)

        self.layout.setRowMinimumHeight(3, 15)

        self.angle_pid_inputs = {"XY": {}, "Z": {}}
        self.angle_pid_inputs["XY"]["P"] = QLineEdit()
        self.angle_pid_inputs["XY"]["P_MAX"] = QLineEdit()
        self.angle_pid_inputs["XY"]["I"] = QLineEdit()
        self.angle_pid_inputs["XY"]["I_MAX"] = QLineEdit()
        self.angle_pid_inputs["Z"]["P"] = QLineEdit()
        self.angle_pid_inputs["Z"]["P_MAX"] = QLineEdit()
        self.angle_pid_inputs["Z"]["I"] = QLineEdit()
        self.angle_pid_inputs["Z"]["I_MAX"] = QLineEdit()

        self.layout.addWidget(QLabel("Stabilize PID"), 4, 0)

        angle_pid_xy_row = 5

        self.layout.addWidget(QLabel("X/Y"), angle_pid_xy_row, 0)

        self.layout.addWidget(QLabel("P"), angle_pid_xy_row, 2)
        self.layout.addWidget(self.angle_pid_inputs["XY"]["P"], angle_pid_xy_row, 3)
        self.layout.addWidget(QLabel("P_MAX"), angle_pid_xy_row, 5)
        self.layout.addWidget(self.angle_pid_inputs["XY"]["P_MAX"],
                angle_pid_xy_row, 6)
        self.layout.addWidget(QLabel("I"), angle_pid_xy_row, 8)
        self.layout.addWidget(self.angle_pid_inputs["XY"]["I"], angle_pid_xy_row, 9)
        self.layout.addWidget(QLabel("I_MAX"), angle_pid_xy_row, 11)
        self.layout.addWidget(self.angle_pid_inputs["XY"]["I_MAX"],
                angle_pid_xy_row, 12)


        angle_pid_z_row = 6
        self.layout.addWidget(QLabel("Z"), angle_pid_z_row, 0)

        self.layout.addWidget(QLabel("P"), angle_pid_z_row, 2)
        self.layout.addWidget(self.angle_pid_inputs["Z"]["P"], angle_pid_z_row, 3)
        self.layout.addWidget(QLabel("P_MAX"), angle_pid_z_row, 5)
        self.layout.addWidget(self.angle_pid_inputs["Z"]["P_MAX"],
                angle_pid_z_row, 6)
        self.layout.addWidget(QLabel("I"), angle_pid_z_row, 8)
        self.layout.addWidget(self.angle_pid_inputs["Z"]["I"], angle_pid_z_row, 9)
        self.layout.addWidget(QLabel("I_MAX"), angle_pid_z_row, 11)
        self.layout.addWidget(self.angle_pid_inputs["Z"]["I_MAX"],
                angle_pid_z_row, 12)

        self.layout.setRowMinimumHeight(7, 15)

        self.loadButton = QPushButton("Load Config")
        self.loadButton.clicked.connect(self.load_config)
        self.saveButton = QPushButton("Save Config")
        self.saveButton.clicked.connect(self.save_config)

        self.layout.addWidget(self.loadButton, 8, 0, 1, 4)
        self.layout.addWidget(self.saveButton, 8, 4, 1, 4)

        self.layout.setRowStretch(9, 1)
        self.setLayout(self.layout)

    def save_config(self):
        print("config saved")

    def load_config(self):
        print("config loaded")

        self.serial_manager.writer.send_packet(SerialReader.REQUEST_CONFIG)

    def config_received(self, data):
        print("widget got config data {}".format(data))
