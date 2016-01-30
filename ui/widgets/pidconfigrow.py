from PySide import QtCore
from PySide.QtGui import *

class PidConfigRow(QWidget):
    def __init__(self, label, key, config):
        super().__init__()

        self.layout = QGridLayout()

        self.layout.setContentsMargins(0, 0, 0, 0)

        self.pid_inputs = {}

        self.pid_inputs["kp"] = QDoubleSpinBox()
        self.pid_inputs["p_max"] = QDoubleSpinBox()
        self.pid_inputs["ki"] = QDoubleSpinBox()
        self.pid_inputs["i_max"] = QDoubleSpinBox()

        for field_key, input in self.pid_inputs.items():
            input.setDecimals(3)
            config.connect("pid_{}_{}".format(key, field_key), input)

        self.layout.setColumnMinimumWidth(1, 15)
        self.layout.setColumnMinimumWidth(4, 15)
        self.layout.setColumnMinimumWidth(7, 15)
        self.layout.setColumnMinimumWidth(10, 15)

        self.layout.setColumnStretch(1, 1)

        self.layout.addWidget(QLabel(label), 0, 0)

        self.layout.addWidget(QLabel("P"), 0, 2)
        self.layout.addWidget(self.pid_inputs["kp"], 0, 3)
        self.layout.addWidget(QLabel("P_MAX"), 0, 5)
        self.layout.addWidget(self.pid_inputs["p_max"], 0, 6)
        self.layout.addWidget(QLabel("I"), 0, 8)
        self.layout.addWidget(self.pid_inputs["ki"], 0, 9)
        self.layout.addWidget(QLabel("I_MAX"), 0, 11)
        self.layout.addWidget(self.pid_inputs["i_max"], 0, 12)

        self.setLayout(self.layout)
