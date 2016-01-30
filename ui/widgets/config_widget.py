from PySide import QtCore
from PySide.QtGui import *
from communication import SerialReader
from pyqtconfig import ConfigManager

class ConfigWidget(QWidget):
    def __init__(self, serialManager):
        super().__init__()

        defaults = {
            "pidRateXYkp": 0.0,
            "pidRateXYpmax": 0.0,
            "pidRateXYki": 0.0,
            "pidRateXYimax": 0.0,
            "pidRateZkp": 0.0,
            "pidRateZpmax": 0.0,
            "pidRateZki": 0.0,
            "pidRateZimax": 0.0
        }

        self.config = ConfigManager()
        self.config.set_defaults(defaults)

        self.serialManager = serialManager

        self.layout = QGridLayout()

        self.layout.setColumnMinimumWidth(1, 15)
        self.layout.setColumnMinimumWidth(4, 15)
        self.layout.setColumnMinimumWidth(7, 15)
        self.layout.setColumnMinimumWidth(10, 15)

        self.ratePidInputs = {"XY": {}, "Z": {}}
        self.ratePidInputs["XY"]["P"] = QDoubleSpinBox()
        self.ratePidInputs["XY"]["P_MAX"] = QDoubleSpinBox()
        self.ratePidInputs["XY"]["I"] = QDoubleSpinBox()
        self.ratePidInputs["XY"]["I_MAX"] = QDoubleSpinBox()
        self.ratePidInputs["Z"]["P"] = QDoubleSpinBox()
        self.ratePidInputs["Z"]["P_MAX"] = QDoubleSpinBox()
        self.ratePidInputs["Z"]["I"] = QDoubleSpinBox()
        self.ratePidInputs["Z"]["I_MAX"] = QDoubleSpinBox()

        self.ratePidInputs["XY"]["P"].setDecimals(3)
        self.ratePidInputs["XY"]["P_MAX"].setDecimals(3)
        self.ratePidInputs["XY"]["I"].setDecimals(3)
        self.ratePidInputs["XY"]["I_MAX"].setDecimals(3)
        self.ratePidInputs["Z"]["P"].setDecimals(3)
        self.ratePidInputs["Z"]["P_MAX"].setDecimals(3)
        self.ratePidInputs["Z"]["I"].setDecimals(3)
        self.ratePidInputs["Z"]["I_MAX"].setDecimals(3)

        self.config.add_handler("pidRateXYkp", self.ratePidInputs["XY"]["P"])
        self.config.add_handler("pidRateXYpmax", self.ratePidInputs["XY"]["P_MAX"])
        self.config.add_handler("pidRateXYki", self.ratePidInputs["XY"]["I"])
        self.config.add_handler("pidRateXYimax", self.ratePidInputs["XY"]["I_MAX"])
        self.config.add_handler("pidRateZkp", self.ratePidInputs["XY"]["P"])
        self.config.add_handler("pidRateZpmax", self.ratePidInputs["XY"]["P_MAX"])
        self.config.add_handler("pidRateZki", self.ratePidInputs["XY"]["I"])
        self.config.add_handler("pidRateZimax", self.ratePidInputs["XY"]["I_MAX"])

        self.layout.addWidget(QLabel("Rate PID"), 0, 0)

        ratePidXYRow = 1

        self.layout.addWidget(QLabel("X/Y"), ratePidXYRow, 0)

        self.layout.addWidget(QLabel("P"), ratePidXYRow, 2)
        self.layout.addWidget(self.ratePidInputs["XY"]["P"], ratePidXYRow, 3)
        self.layout.addWidget(QLabel("P_MAX"), ratePidXYRow, 5)
        self.layout.addWidget(self.ratePidInputs["XY"]["P_MAX"], ratePidXYRow, 6)
        self.layout.addWidget(QLabel("I"), ratePidXYRow, 8)
        self.layout.addWidget(self.ratePidInputs["XY"]["I"], ratePidXYRow, 9)
        self.layout.addWidget(QLabel("I_MAX"), ratePidXYRow, 11)
        self.layout.addWidget(self.ratePidInputs["XY"]["I_MAX"], ratePidXYRow, 12)


        ratePidZRow = 2
        self.layout.addWidget(QLabel("Z"), ratePidZRow, 0)

        self.layout.addWidget(QLabel("P"), ratePidZRow, 2)
        self.layout.addWidget(self.ratePidInputs["Z"]["P"], ratePidZRow, 3)
        self.layout.addWidget(QLabel("P_MAX"), ratePidZRow, 5)
        self.layout.addWidget(self.ratePidInputs["Z"]["P_MAX"], ratePidZRow, 6)
        self.layout.addWidget(QLabel("I"), ratePidZRow, 8)
        self.layout.addWidget(self.ratePidInputs["Z"]["I"], ratePidZRow, 9)
        self.layout.addWidget(QLabel("I_MAX"), ratePidZRow, 11)
        self.layout.addWidget(self.ratePidInputs["Z"]["I_MAX"], ratePidZRow, 12)

        self.layout.setRowMinimumHeight(3, 15)

        self.anglePidInputs = {"XY": {}, "Z": {}}
        self.anglePidInputs["XY"]["P"] = QLineEdit()
        self.anglePidInputs["XY"]["P_MAX"] = QLineEdit()
        self.anglePidInputs["XY"]["I"] = QLineEdit()
        self.anglePidInputs["XY"]["I_MAX"] = QLineEdit()
        self.anglePidInputs["Z"]["P"] = QLineEdit()
        self.anglePidInputs["Z"]["P_MAX"] = QLineEdit()
        self.anglePidInputs["Z"]["I"] = QLineEdit()
        self.anglePidInputs["Z"]["I_MAX"] = QLineEdit()

        self.layout.addWidget(QLabel("Stabilize PID"), 4, 0)

        anglePidXYRow = 5

        self.layout.addWidget(QLabel("X/Y"), anglePidXYRow, 0)

        self.layout.addWidget(QLabel("P"), anglePidXYRow, 2)
        self.layout.addWidget(self.anglePidInputs["XY"]["P"], anglePidXYRow, 3)
        self.layout.addWidget(QLabel("P_MAX"), anglePidXYRow, 5)
        self.layout.addWidget(self.anglePidInputs["XY"]["P_MAX"], anglePidXYRow, 6)
        self.layout.addWidget(QLabel("I"), anglePidXYRow, 8)
        self.layout.addWidget(self.anglePidInputs["XY"]["I"], anglePidXYRow, 9)
        self.layout.addWidget(QLabel("I_MAX"), anglePidXYRow, 11)
        self.layout.addWidget(self.anglePidInputs["XY"]["I_MAX"], anglePidXYRow, 12)


        anglePidZRow = 6
        self.layout.addWidget(QLabel("Z"), anglePidZRow, 0)

        self.layout.addWidget(QLabel("P"), anglePidZRow, 2)
        self.layout.addWidget(self.anglePidInputs["Z"]["P"], anglePidZRow, 3)
        self.layout.addWidget(QLabel("P_MAX"), anglePidZRow, 5)
        self.layout.addWidget(self.anglePidInputs["Z"]["P_MAX"], anglePidZRow, 6)
        self.layout.addWidget(QLabel("I"), anglePidZRow, 8)
        self.layout.addWidget(self.anglePidInputs["Z"]["I"], anglePidZRow, 9)
        self.layout.addWidget(QLabel("I_MAX"), anglePidZRow, 11)
        self.layout.addWidget(self.anglePidInputs["Z"]["I_MAX"], anglePidZRow, 12)

        self.layout.setRowMinimumHeight(7, 15)

        self.loadButton = QPushButton("Load Config")
        self.loadButton.clicked.connect(self.loadConfig)
        self.saveButton = QPushButton("Save Config")
        self.saveButton.clicked.connect(self.saveConfig)

        self.layout.addWidget(self.loadButton, 8, 0, 1, 4)
        self.layout.addWidget(self.saveButton, 8, 4, 1, 4)

        self.layout.setRowStretch(9, 1)
        self.setLayout(self.layout)

    def saveConfig(self):
        print("config saved")

    def loadConfig(self):
        print("config loaded")

        self.serialManager.writer.sendPacket(SerialReader.REQUEST_CONFIG)
