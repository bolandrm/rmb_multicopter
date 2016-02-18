import _ from "underscore";
import SerialReader from "./serial_reader";
import SerialWriter from "./serial_writer";

class SerialManager {
  openPortId = -1;
  connectionListeners = [];
  callbacks = {};

  constructor() {
    this.reader = new SerialReader(this.dataReceieved);
    this.writer = new SerialWriter(this.rawSendData);
    // chrome.serial.onReceive.addListener(this.handleData);
    // setInterval(this.checkConnectionStatus, 500);
  }

  connect(port, callback) {
    chrome.serial.connect(port, { bitrate: 115200 }, (connectionInfo) => {
      if (connectionInfo) {
        console.log("connected");
        console.log(connectionInfo);
        callback(connectionInfo.connectionId)
      } else {
        console.log("failed to connect");
        callback(false)
      }
    });
  }

  disconnect() {
    if (this.openPortId === -1) { return; }

    this.clearCallbacks();
    chrome.serial.disconnect(this.openPortId, (success) => {
      if (success) {
        console.log("disconnected");
        this.connectionStatus("disconnected");
        this.openPortId = -1;
      } else {
        console.log("failed to disconnect");
      }
    });
  }

  rawSendData = (packetBuffer) => {
    if (this.openPortId === -1) {
      console.log("no ports open");
      return;
    }

    chrome.serial.send(this.openPortId, packetBuffer, (sendInfo) => {
      // console.log(sendInfo);
      if (sendInfo.error) { this.disconnect(); }
    });
  }

  send = (code, data, callback) => {
    if (this.openPortId === -1) { return; }

    if (callback) {
      let callbackWrapper = this.callbacks[code];

      if (!callbackWrapper) {
        callbackWrapper = { handlers: [] }
        callbackWrapper.timer = setTimeout(() => {
          console.log(`request timed out: ${code}`);
        }, 1000);
        this.callbacks[code] = callbackWrapper;
      }

      callbackWrapper.handlers.push(callback);
    }

    this.writer.sendPacket(code, data);
  }

  dataReceieved = (code, data) => {
    let callback = this.callbacks[code];

    if (callback) {
      _.each(callback.handlers, function(handler) {
        handler(data);
      });

      clearTimeout(callback.timer);
      this.callbacks = _.omit(this.callbacks, code);
    }
  }

  handleData = (info) => {
    const data = new Uint8Array(info.data);

    data.forEach((value) => {
      this.reader.processCommand(value);
    });
  }

  getDevices(callback) {
    chrome.serial.getDevices(function(ports) {
      ports = _.map(ports, function(port) { return port.path; });

      ports = _.filter(ports, function(port) {
        return !port.match(/[Bb]luetooth/) && port.match(/\/dev\/tty/);
      });

      callback(ports);
    });
  }

  addConnectionStatusListener(listener) {
    this.connectionListeners.push(listener);
  }

  checkConnectionStatus = () => {
    if (this.openPortId === -1) { return; }
    this.rawSendData(new ArrayBuffer(1));
  }

  clearCallbacks() {
    _.mapObject(this.callbacks, function(key, callback) {
      clearTimeout(callback.timer);
    });
    this.callbacks = {};
  }
}

const serial_manager = new SerialManager();

export default serial_manager;
