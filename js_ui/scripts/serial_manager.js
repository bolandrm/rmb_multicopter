import SerialReader from "./serial_reader";
import SerialWriter from "./serial_writer";

class SerialManager {
  openPortId = -1;

  constructor() {
    this.connect();
    this.reader = new SerialReader();
    this.writer = new SerialWriter(this.sendData);
  }

  sendData = (packetBuffer) => {
    chrome.serial.send(this.openPortId, packetBuffer, (sendInfo) => {
      this.log(sendInfo);
    });
  }

  connectToPort(port) {
    chrome.serial.connect(port, { bitrate: 115200 }, (connectionInfo) => {
      const connectionId = connectionInfo.connectionId;

      if (connectionId === -1) {
        this.log("could not open");
      } else {
        this.openPortId = connectionId;
        this.log("opened port");
      }
    });
  }

  handleData = (info) => {
    const data = new Uint8Array(info.data);

    data.forEach((value) => {
      this.reader.processCommand(value);
    });
  }

  connect() {
    chrome.serial.getDevices((p) => { this.log(p) });
    this.connectToPort("/dev/cu.usbmodem747851");
    chrome.serial.onReceive.addListener(this.handleData);
  }

  log(text) {
    return;
    console.log(text);
  }
}

const serial_manager = new SerialManager();

export default serial_manager;
