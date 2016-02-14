cfg.Serial = class Serial {
  openPortId = -1;

  log(text) {
    return;
    console.log(text);
  }

  connectToPort(port) {
    chrome.serial.connect(port, { bitrate: 115200 }, (connectionInfo) => {
      const connectionId = connectionInfo.connectionId;

      if (connectionId === -1) {
        console.log("could not open");
      } else {
        this.openPortId = connectionId;
        console.log("opened port");
      }
    });
  }

  code = 0;
  state = 0;
  incomingCrc = 0;
  dataExpectedLength = 0;
  dataReceivedLength = 0;
  dataBuffer = null;
  dataBufferView = null;

  processCommand(data) {
    switch(this.state) {
      case 0:
        if (data === cfg.SerialCodes.PACKET_HEADER1) {
          this.state++;
          this.log("got header 1");
        }
        break;
      case 1:
        if (data === cfg.SerialCodes.PACKET_HEADER2) {
          this.state++;
          this.log("got header 2");
        } else {
          this.state = 0;
        }
        break;
      case 2:
        this.code = data;
        this.incomingCrc = data;
        this.state++;
        this.log(`got code: ${data}`);
        break;
      case 3:  // Data length LSB
        this.dataExpectedLength = data;
        this.incomingCrc ^= data;
        this.state++;
        break;
      case 4:  // Data length MSB
        this.dataExpectedLength |= (data << 8);
        this.incomingCrc ^= data;
        this.log(`data length: ${this.dataExpectedLength}`);
        this.dataBuffer = new ArrayBuffer(this.dataExpectedLength);
        this.dataBufferView = new Uint8Array(this.dataBuffer);
        this.state++;
        break;
      case 5:
        this.dataBufferView[this.dataReceivedLength++] = data;
        this.incomingCrc ^= data;
        if (this.dataReceivedLength >= this.dataExpectedLength) {
          this.state++;
        }
        break;
      case 6:
        this.log(`data length received: ${this.dataReceivedLength}`);
        this.log(`incoming crc: ${data} -- calc'd crc: ${this.incomingCrc}`);

        if (this.incomingCrc === data) {
          this.log("crc match!");

          switch(this.code) {
            case cfg.SerialCodes.REQUEST_CONFIG:
              console.log(cfg.Struct.parse(this.dataBuffer, cfg.StructLayouts.config));
              break;
            case cfg.SerialCodes.REQUEST_GYRO_ACC:
              console.log(cfg.Struct.parse(this.dataBuffer, cfg.StructLayouts.gyroAcc));
              break;
            case cfg.SerialCodes.INFO_SUCCESS:
              console.log("controller responded with success!");
              break;
            case cfg.SerialCodes.INFO_FAILURE:
              console.log("controller responded with failure");
              break;
            default:
              console.log(`unknown code ${this.code}`);
          }
        } else {
          console.log("bad crc!");
        }

        this.code = 0;
        this.state = 0;
        this.incomingCrc = 0;
        this.dataExpectedLength = 0;
        this.dataReceivedLength = 0;
        this.dataBuffer = null;
        this.dataBufferView = null;
        break;
    }
  }

  handleData = (info) => {
    const data = new Uint8Array(info.data);

    data.forEach((value) => {
      this.processCommand(value);
    });
  }

  sendPacket(code, data = null) {
    if (!data) { data = new ArrayBuffer(1); }
    const dataView = new Uint8Array(data);

    const size = 6 + data.byteLength;
    let crc = 0;

    let packetBuffer = new ArrayBuffer(size);
    let packetView = new Uint8Array(packetBuffer); 

    packetView[0] = cfg.SerialCodes.PACKET_HEADER1;
    packetView[1] = cfg.SerialCodes.PACKET_HEADER2;
    packetView[2] = code;
    packetView[3] = cfg.Utils.lowByte(data.byteLength);
    packetView[4] = cfg.Utils.highByte(data.byteLength);

    crc = packetView[2] ^ packetView[3] ^ packetView[4];

    dataView.forEach(function(d, i) {
      packetView[i+5] = d;
      crc ^= d;
    });

    packetView[size-1] = crc;

    chrome.serial.send(this.openPortId, packetBuffer, (sendInfo) => {
      this.log(packetView);
      this.log(sendInfo);
    });
  }

  autoConnect() {
    chrome.serial.getDevices(this.log);
    this.connectToPort("/dev/cu.usbmodem747851");
    chrome.serial.onReceive.addListener(this.handleData);
  }
}

//   $("#connect-button").click(function() {
//     if (connectionId !== -1) {
//       chrome.serial.disconnect(connectionId, function(){});
//       connectionId = -1;
//     } else {
//       openSelectedPort();
//     }
//   });
// 
//     ports = _.filter(ports, function(port) {
//       return !port.match(/[Bb]luetooth/) && port.match(/\/dev\/tty/);
//     });
// 
//     portPicker.change(function() {
//       if (connectionId != -1) {
//         chrome.serial.disconnect(connectionId, openSelectedPort);
//       } else {
//         openSelectedPort();
//       }
//     });
//   }
