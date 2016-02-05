from PySide.QtCore import QThread, Signal
from PySide.QtGui import *
import serial.tools.list_ports
import time
import comms

class RefreshThread(QThread):
    is_finished = False

    def __init__(self, config_header):
        super().__init__()
        self.config_header = config_header
        self.start()

    def run(self):
        while self.is_finished == False:
            self.config_header.refresh()
            time.sleep(0.5)

    def finished(self):
        self.is_finished = True

class ConfigHeader(QFrame):
    def __init__(self):
        super().__init__()

        self.port_selector = QComboBox()
        self.connect_button = QPushButton("Connect")
        self.connect_button.clicked.connect(self.connect_to_port)
        self.refresh_ports_button = QPushButton("Refresh Ports")
        self.refresh_ports_button.clicked.connect(self.refresh_port_list)

        self.layout = QHBoxLayout()
        self.layout.addWidget(QLabel("Port:"))
        self.layout.addWidget(self.port_selector)
        self.layout.addWidget(self.refresh_ports_button)
        self.layout.addWidget(self.connect_button)
        self.layout.addStretch(1)

        self.refresh_port_list()

        self.setFrameStyle(QFrame.StyledPanel)
        self.setLayout(self.layout)

        self.refresh_thread = RefreshThread(self)

    def refresh_port_list(self):
        self.port_selector.clear()
        for port in serial.tools.list_ports.comports():
            self.port_selector.addItem(port.device)
        self.choose_default_port()

    def connect_to_port(self):
        if comms.SerialManager().serial_port.isOpen():
            comms.SerialManager().close_port()
        else:
            comms.SerialManager().open_port(self.port_selector.currentText())
        self.refresh()

    def choose_default_port(self):
        for i in range(self.port_selector.count()):
            if "usbmodem" in self.port_selector.itemText(i):
                self.port_selector.setCurrentIndex(i)
                return

    def refresh(self):
        if comms.SerialManager().serial_port.isOpen():
            if self.connect_button.text != "Disconnect":
                self.connect_button.setText("Disconnect")
        else:
            if self.connect_button.text != "Connect":
                self.connect_button.setText("Connect")

    def stop(self):
        print("Cleaning up config header thread")
        while self.refresh_thread.isRunning():
            self.refresh_thread.finished()
