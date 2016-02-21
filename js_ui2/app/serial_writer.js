import Utils from './utils'
import SerialCodes from './serial_codes'

class SerialWriter {
  constructor(send) {
    this.send = send
  }

  sendPacket(code, data = null, callback = null) {
    if (!data) { data = new ArrayBuffer(1) }
    const dataView = new Uint8Array(data)

    const size = 6 + data.byteLength
    let crc = 0

    let packetBuffer = new ArrayBuffer(size)
    let packetView = new Uint8Array(packetBuffer) 

    packetView[0] = SerialCodes.PACKET_HEADER1
    packetView[1] = SerialCodes.PACKET_HEADER2
    packetView[2] = code
    packetView[3] = Utils.lowByte(data.byteLength)
    packetView[4] = Utils.highByte(data.byteLength)

    crc = packetView[2] ^ packetView[3] ^ packetView[4]

    dataView.forEach(function(d, i) {
      packetView[i+5] = d
      crc ^= d
    })

    packetView[size-1] = crc

    // console.log('sending packet', packetView)
    this.send(packetBuffer)
  }
}

export default SerialWriter
