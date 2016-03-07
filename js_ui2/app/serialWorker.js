'use strict'

const _ = require('lodash')
const SerialPort = require('serialport')

let currentSerialPort = null

const onDataReceived = function(data) {
  process.send(['data', { buffer: data }])
}

const onClose = function() {
  console.log('closing or disconnected')
  process.exit()
}

const onError = function(e) {
  console.log('port error', e)
  process.exit()
}

process.on('message', function(payload) {
  const type = payload[0]
  const params = payload[1]

  switch (type) {
    case 'connect':
      currentSerialPort = new SerialPort.SerialPort(
        params.port,
        {
          baudRate: 115200,
          disconnectedCallback: onClose
        },
        false
      )

      currentSerialPort.on('data', onDataReceived)
      currentSerialPort.on('close', onClose)
      currentSerialPort.on('error', onError)

      currentSerialPort.open((error) => {
        if (error) {
          process.exit()
        } else {
          currentSerialPort.resume()
          process.send(['connect'])
        }
      })

      break

    case 'disconnect':
      if (currentSerialPort) { 
        currentSerialPort.close((error) => {
          if (error) console.log('failed to disconnect')
          process.send(['disconnect'])
        })
      }

      process.exit()

      break

    case 'send':
      let packetView = new Uint8Array(params.data) 
      currentSerialPort.write(packetView, (error) => {
        if (error) onError()
      })
      break

    default:
      console.log('unknown type', type)
  }
})
