import struct
from libs.pyqtconfig import ConfigManager
from collections import OrderedDict
import comms

CONFIG_VERSION = 2

CONFIG_MAP = [
    ("version", "H"),

    ("pid_rate_xy_kp", "f"),
    ("pid_rate_xy_p_max", "f"),
    ("pid_rate_xy_ki", "f"),
    ("pid_rate_xy_i_max", "f"),

    ("pid_rate_xy_kp", "f"),
    ("pid_rate_xy_p_max", "f"),
    ("pid_rate_xy_ki", "f"),
    ("pid_rate_xy_i_max", "f"),

    ("pid_rate_z_kp", "f"),
    ("pid_rate_z_p_max", "f"),
    ("pid_rate_z_ki", "f"),
    ("pid_rate_z_i_max", "f"),

    ("pid_angle_xy_kp", "f"),
    ("pid_angle_xy_p_max", "f"),
    ("pid_angle_xy_ki", "f"),
    ("pid_angle_xy_i_max", "f"),

    ("pid_angle_xy_kp", "f"),
    ("pid_angle_xy_p_max", "f"),
    ("pid_angle_xy_ki", "f"),
    ("pid_angle_xy_i_max", "f")
]

class ConfigSync(object):
    def __init__(self, config_widget):
        comms.SerialManager().reader.on_config_received.connect(self.config_received)

        self.config_widget = config_widget

        defaults = {}

        for k, v in CONFIG_MAP:
            if v == "f": defaults[k] = 0.0
            if v == "H": defaults[k] = 0

        self.manager = ConfigManager()
        self.manager.set_defaults(defaults)

    def connect(self, key, input):
        self.manager.add_handler(key, input)

    def struct_arrangement(self):
        arrangement = [x[1] for x in CONFIG_MAP]
        str = "< {}".format("".join(arrangement))
        return str

    def config_received(self, data_buffer):
        print("unpacking config data")
        data = struct.unpack(self.struct_arrangement(), data_buffer)
        print("widget got config data {}".format(data))

        if (data[0] != CONFIG_VERSION):
            print("incorrect config version")
            return

        new_config = {}

        for i, (k, v) in enumerate(CONFIG_MAP):
            new_config[k] = data[i]

        self.manager.set_many(new_config)
        self.config_widget.config_loaded()

    def save_config(self):
        print("saving config")
        keys = [x[0] for x in CONFIG_MAP]
        data = [self.manager.get(k) for k in keys]
        data_buffer = struct.pack(self.struct_arrangement(), *data)
        comms.SerialManager().writer.send_packet(comms.SET_CONFIG, data_buffer)

    def load_config(self):
        print("loading config")
        comms.SerialManager().writer.send_packet(comms.REQUEST_CONFIG)
