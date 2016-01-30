from libs.pyqtconfig import ConfigManager

class ConfigSync(object):
    PIDS = ["rate_xy", "rate_z", "angle_xy", "angle_z"]

    def __init__(self):
        defaults = {}

        for pid in self.PIDS:
            defaults["pid_{}_kp".format(pid)] = 5.0
            defaults["pid_{}_p_max".format(pid)] = 0.0
            defaults["pid_{}_ki".format(pid)] = 3.0
            defaults["pid_{}_i_max".format(pid)] = 0.0

        self.manager = ConfigManager()
        self.manager.set_defaults(defaults)

    def connect(self, key, input):
        self.manager.add_handler(key, input)
