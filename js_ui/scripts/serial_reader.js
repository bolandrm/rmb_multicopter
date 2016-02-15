import SerialCodes from "./serial_codes";
import Struct from "./struct";
import StructLayouts from "./struct_layouts";

class SerialReader {
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
        if (data === SerialCodes.PACKET_HEADER1) {
          this.state++;
          this.log("got header 1");
        }
        break;
      case 1:
        if (data === SerialCodes.PACKET_HEADER2) {
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
            case SerialCodes.REQUEST_CONFIG:
              console.log(Struct.parse(this.dataBuffer, StructLayouts.config));
              break;
            case SerialCodes.REQUEST_GYRO_ACC:
              console.log(Struct.parse(this.dataBuffer, StructLayouts.gyroAcc));
              break;
            case SerialCodes.INFO_SUCCESS:
              console.log("controller responded with success!");
              break;
            case SerialCodes.INFO_FAILURE:
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

  log(text) {
    return;
    console.log(text);
  }
}

export default SerialReader;

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
