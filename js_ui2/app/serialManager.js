import _ from 'lodash'
import { observable } from 'mobx'
import SerialReader from './serial_reader'
import SerialWriter from './serial_writer'
import SerialPort from 'serialport'
import SerialCodes from './serial_codes'

class SerialManager {
  @observable ports = []
  @observable portSelected
  @observable connected = false
  @observable busy = false
  currentSerialPort = null
  callbacks = {}

  constructor() {
    this.reader = new SerialReader(this._dataParsed)
    this.writer = new SerialWriter(this._rawSendData)

    this.refreshPorts(true)
  }

  isDefaultDevice(device) {
    return device.match(/usbmodem/)
  }

  findDefaultDevice(devices) {
    return _.find(devices, (device) => this.isDefaultDevice(device))
  }

  refreshPorts = (autoconnect = false) => {
    SerialPort.list((error, ports) => {
      if (error) {
        console.log('failed to get devices')
      } else {
        ports = _.map(ports, (port) => port.comName)
        ports = _.filter(ports, (port) => !port.match(/[Bb]luetooth/) && port.match(/\/dev\/cu/))
        this.ports = ports

        let defaultDevice = this.findDefaultDevice(this.ports)

        if (defaultDevice) {
          this.portSelected = defaultDevice
          if (autoconnect === true) this.connect()
        } else {
          this.portSelected = this.ports[0]
        }
      }
    })
  }

  connect = () => {
    this.busy = true

    this.currentSerialPort = new SerialPort.SerialPort(
      this.portSelected,
      {
        baudRate: 115200,
        disconnectedCallback: this._onClose
      },
      false
    )

    this.currentSerialPort.on('data', this._onDataReceived)
    this.currentSerialPort.on('close', this._onClose)
    this.currentSerialPort.on('error', this._onError)

    this.currentSerialPort.open((error) => {
      if (error) {
        console.log('failed to connect')
        this.currentSerialPort = null
        this.connected = false
      } else {
        console.log('connected')
        this.currentSerialPort.resume()
        this.connected = true
      }

      this.busy = false
    })
  }

  disconnect = () => {
    if (!this.currentSerialPort) { return this.connected = false }

    this.currentSerialPort.close((error) => {
      if (error) { console.log('failed to disconnect') }
      this.connected = false
    })
  }

  send = (code, data, callback) => {
    if (!this.connected) return

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

    this.writer.sendPacket(code, data)
  }

  _onClose = () => {
    console.log('closing or disconnected')
    this._clearCallbacks()
    this.connected = false
  }

  _onDataReceived = (data) => {
    var data = new Uint8Array(data)

    data.forEach((value) => {
      this.reader.processCommand(value)
    })
  }

  _onError = (e) => {
    console.log('port error', e)
    this.disconnect()
  }

  _dataParsed = (code, data) => {
    console.log('data parsed', code, data)

    let callback = this.callbacks[code];

    if (callback) {
      _.each(callback.handlers, function(handler) {
        handler(data);
      });

      clearTimeout(callback.timer);
      this.callbacks = _.omit(this.callbacks, code);
    }
  }

  _rawSendData = (packetBuffer) => {
    if (!this.connected) {
      console.log('no ports open')
      return
    }

    this.currentSerialPort.write(packetBuffer, (error) => {
      if (error) this._onError()
    })
  }

  _clearCallbacks() {
    _.forOwn(this.callbacks, function(key, callback) {
      clearTimeout(callback.timer);
    });
    this.callbacks = {};
  }
}

const serialManager = new SerialManager()

export default serialManager
