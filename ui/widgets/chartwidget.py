import PySide
import pyqtgraph as pg
import numpy as np
import comms

class ChartWidget(pg.GraphicsLayoutWidget):
    def __init__(self):
        super().__init__()

        comms.SerialManager().reader.on_gyro_acc_received.connect(self.update_data)

        self.plot = self.addPlot()
        self.plot.setRange(xRange=[0,300])
        self.plot.setMouseEnabled(x=False)

        self.data = np.zeros(300)

        self.curve = self.plot.plot(self.data)

    def update_data(self, imu_data):
        self.data = np.roll(self.data, 1)
        self.data[0] = imu_data[3]
        self.curve.setData(self.data)
