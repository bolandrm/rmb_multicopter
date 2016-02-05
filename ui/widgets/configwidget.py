from PySide import QtCore
from PySide.QtGui import *

from configsync import ConfigSync
from .pidconfigrow import PidConfigRow

class ConfigWidget(QFrame):
    def __init__(self):
        super().__init__()

        self.config = ConfigSync(self)

        self.setFixedWidth(550)

        self.layout = QVBoxLayout()

        self.layout.addWidget(QLabel("Rate PID"))

        self.rate_pid_xy_row = PidConfigRow("X/Y", key="rate_xy", config=self.config)
        self.layout.addWidget(self.rate_pid_xy_row)

        self.rate_pid_z_row = PidConfigRow("Z", key="rate_z", config=self.config)
        self.layout.addWidget(self.rate_pid_z_row)

        self.layout.addWidget(QLabel("Angle PID"))

        self.angle_pid_xy_row = PidConfigRow("X/Y", key="angle_xy", config=self.config)
        self.layout.addWidget(self.angle_pid_xy_row)

        loadButton = QPushButton("Fetch Config")
        loadButton.clicked.connect(self.config.load_config)
        self.saveButton = QPushButton("Write Config")
        self.saveButton.setEnabled(False)
        self.saveButton.clicked.connect(self.config.save_config)

        self.buttonLayout = QHBoxLayout()
        self.buttonLayout.addWidget(loadButton)
        self.buttonLayout.addWidget(self.saveButton)
        self.layout.addLayout(self.buttonLayout)

        self.layout.addStretch(1)
        self.setLayout(self.layout)

        self.setFrameStyle(QFrame.StyledPanel)

    def config_loaded(self):
        self.saveButton.setEnabled(True)
