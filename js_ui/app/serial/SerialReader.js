import * as serialCodes from './serialCodes'
import { parseStruct } from "./struct";
import * as structLayouts from "./structLayouts";

class SerialReader {
  code = 0;
  state = 0;
  incomingCrc = 0;
  dataExpectedLength = 0;
  dataReceivedLength = 0;
  dataBuffer = null;
  dataBufferView = null;

  constructor(dataReceived) {
    this.dataReceived = dataReceived;
  }

  processCommand(data) {
    switch(this.state) {
      case 0:
        if (data === serialCodes.PACKET_HEADER1) {
          this.state++;
          this.log("got header 1");
        }
        break;
      case 1:
        if (data === serialCodes.PACKET_HEADER2) {
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

          var response;

          switch(this.code) {
            case serialCodes.REQUEST_CONFIG:
              response = parseStruct(this.dataBuffer, structLayouts.config);
              break;
            case serialCodes.REQUEST_GYRO_ACC:
              response = parseStruct(this.dataBuffer, structLayouts.gyroAcc);
              break;
            case serialCodes.REQUEST_RC:
              response = parseStruct(this.dataBuffer, structLayouts.rcData);
              break;
            case serialCodes.INFO_SUCCESS:
              console.log("controller responded with success!");
              break;
            case serialCodes.INFO_FAILURE:
              console.log("controller responded with failure");
              break;
            default:
              console.log(`unknown code ${this.code}`);
          }

          this.dataReceived(this.code, response);

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
