import _ from "lodash";
import SerialReader from "./serial_reader";
import SerialWriter from "./serial_writer";
import serialport from 'serialport'

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

  connect(port) {
    return new Promise(function (resolve, reject) {
      chrome.serial.connect(port, { bitrate: 115200 }, (connectionInfo) => {
        if (connectionInfo) {
          console.log("connected")
          console.log(connectionInfo)
          resolve(connectionInfo.connectionId)
        } else {
          console.log("failed to connect")
          reject()
        }
      })
    })
  }

  disconnect(port) {
    return new Promise(function (resolve, reject) {
      this.clearCallbacks();

      chrome.serial.disconnect(port, (success) => {
        if (success) {
          console.log("disconnected");
        } else {
          console.log("failed to disconnect");
        }

        resolve()
      });
    })
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

  getDevices() {
    return new Promise(function (resolve, reject) {
      serialport.list(function(error, ports) {
        if (error) {
          reject()
        } else {
          ports = _.map(ports, function(port) { return port.comName; })

          console.log(ports)

          ports = _.filter(ports, function(port) {
            return !port.match(/[Bb]luetooth/) && port.match(/\/dev\/cu/)
          });

          resolve(ports);
        }
      });
    })
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
