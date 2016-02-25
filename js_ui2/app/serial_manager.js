import _ from 'lodash'
import SerialReader from './serial_reader'
import SerialWriter from './serial_writer'
import SerialPort from 'serialport'
import store from './store'
import SerialCodes from './serial_codes'
import * as actions from './actions'

class SerialManager {
  currentSerialPort = null

  constructor() {
    this.reader = new SerialReader(this._dataParsed)
    this.writer = new SerialWriter(this._rawSendData)

    this.invertedSerialCodes = _.invert(SerialCodes)
    _.forOwn(this.invertedSerialCodes, (v, k) => {
      let newVal = this.invertedSerialCodes[k].replace("REQUEST_", "")
      this.invertedSerialCodes[k] = `SERIAL_GOT_${newVal}`
    })
  }

  getDevices() {
    return new Promise(function (resolve, reject) {
      SerialPort.list(function(error, ports) {
        if (error) {
          reject()
        } else {
          ports = _.map(ports, function(port) { return port.comName })

          ports = _.filter(ports, function(port) {
            return !port.match(/[Bb]luetooth/) && port.match(/\/dev\/cu/)
          })

          resolve(ports)
        }
      })
    })
  }

  connect = (port) => {
    return new Promise((resolve, reject) => {
      this.currentSerialPort = new SerialPort.SerialPort(
        port,
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
          reject(false)
        } else {
          console.log('connected')
          this.currentSerialPort.resume()
          resolve(true)
        }
      })
    })
  }

  disconnect = () => {
    return new Promise((resolve, reject) => {
      if (!this.currentSerialPort) { resolve() }

      this.currentSerialPort.close((error) => {
        if (error) { console.log('failed to disconnect') }
        resolve()
      })
    })
  }

  send = (code, data) => {
    if (!this.currentSerialPort) return
    this.writer.sendPacket(code, data)
  }

  _onClose = () => {
    console.log('closing or disconnected')
    store.dispatch(actions.disconnected())
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
    store.dispatch(actions.disconnected())
  }

  _dataParsed = (code, data) => {
    const codeName = this.invertedSerialCodes[code]
    store.dispatch(actions.dataParsed(codeName, data))
  }

  _rawSendData = (packetBuffer) => {
    if (!this.currentSerialPort) {
      console.log('no ports open')
      return
    }

    this.currentSerialPort.write(packetBuffer, (error) => {
      if (error) this._onError()
    })
  }
}

const serial_manager = new SerialManager()

export default serial_manager
