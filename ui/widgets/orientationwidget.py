from PySide import QtCore, QtGui, QtOpenGL
from OpenGL import GL

class OrientationWidget(QtOpenGL.QGLWidget):
    def __init__(self):
        QtOpenGL.QGLWidget.__init__(self)

        print("widget init")
        self.object = 0
        self.xRot = 0
        self.yRot = 0
        self.zRot = 0

        self.colorBlue = QtGui.QColor.fromRgb(0, 0, 240)
        self.colorGray = QtGui.QColor.fromRgb(177, 177, 177)
        self.colorGreen = QtGui.QColor.fromRgb(0, 240, 0)
        self.colorWhite = QtGui.QColor.fromRgb(255, 255, 255)

    def xRotation(self):
        return self.xRot

    def yRotation(self):
        return self.yRot

    def zRotation(self):
        return self.zRot

    def minimumSizeHint(self):
        return QtCore.QSize(50, 50)

    def sizeHint(self):
        return QtCore.QSize(400, 400)

    def setXRotation(self, angle):
        angle = (angle + 90) * 16
        if angle != self.xRot:
            self.xRot = angle
            self.updateGL()

    def setYRotation(self, angle):
        angle = angle * 16
        if angle != self.yRot:
            self.yRot = angle
            self.updateGL()

    def setZRotation(self, angle):
        angle = (angle + 135) * 16
        if angle != self.zRot:
            self.zRot = angle
            self.updateGL()

    def initializeGL(self):
        self.qglClearColor(self.colorWhite)
        self.object = self.makeObject()
        GL.glShadeModel(GL.GL_FLAT)
        GL.glEnable(GL.GL_DEPTH_TEST)
        GL.glEnable(GL.GL_CULL_FACE)

    def paintGL(self):
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
        GL.glLoadIdentity()
        GL.glTranslated(0.0, 0.0, -10.0)
        GL.glRotated(self.xRot / 16.0, 1.0, 0.0, 0.0)
        GL.glRotated(self.yRot / 16.0, 0.0, 1.0, 0.0)
        GL.glRotated(self.zRot / 16.0, 0.0, 0.0, 1.0)
        GL.glCallList(self.object)

    def resizeGL(self, width, height):
        side = min(width, height)
        GL.glViewport(int((width - side) / 2), int((height - side) / 2), side, side)

        GL.glMatrixMode(GL.GL_PROJECTION)
        GL.glLoadIdentity()
        GL.glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0)
        GL.glMatrixMode(GL.GL_MODELVIEW)

    def makeObject(self):
        genList = GL.glGenLists(1)
        GL.glNewList(genList, GL.GL_COMPILE)

        GL.glBegin(GL.GL_QUADS)

        x1 = +0.35
        y1 = -0.10
        x2 = +0.10
        y2 = -0.35

        self.qglColor(self.colorBlue)
        self.quad(x1, y1, x2, y2, y2, x2, y1, x1)

        self.qglColor(self.colorGreen)
        self.extrude(x1, y1, x2, y2)
        self.extrude(y2, x2, y1, x1)

        self.qglColor(self.colorGray)
        self.extrude(x2, y2, y2, x2)
        self.extrude(y1, x1, x1, y1)

        GL.glEnd()
        GL.glEndList()

        return genList

    def quad(self, x1, y1, x2, y2, x3, y3, x4, y4):
        GL.glVertex3d(x1, y1, +0.07)
        GL.glVertex3d(x2, y2, +0.07)
        GL.glVertex3d(x3, y3, +0.07)
        GL.glVertex3d(x4, y4, +0.07)

        GL.glVertex3d(x4, y4, -0.07)
        GL.glVertex3d(x3, y3, -0.07)
        GL.glVertex3d(x2, y2, -0.07)
        GL.glVertex3d(x1, y1, -0.07)

    def extrude(self, x1, y1, x2, y2):
        GL.glVertex3d(x1, y1, -0.07)
        GL.glVertex3d(x2, y2, -0.07)
        GL.glVertex3d(x2, y2, +0.07)
        GL.glVertex3d(x1, y1, +0.07)

#def update_imu_data(imu_data):
#    (imu_rate_x, imu_rate_y, imu_rate_z,
#    imu_angles_x, imu_angles_y, imu_angles_z) = imu_data
#
#    window.glWidget.setXRotation(-imu_angles_y)
#    window.glWidget.setYRotation(imu_angles_x)
#
#    print("imu data: {}", imu_data)
