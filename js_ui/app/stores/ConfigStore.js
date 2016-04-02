import { observable, toJSON } from 'mobx'
import serial from '../serial/serialManager'
import * as structLayouts from "../serial/structLayouts"
import { buildStruct } from '../serial/struct'

const CONFIG_VERSION = 6

export default class ConfigStore {
  @observable fetchedOnce = false
  @observable data = {
    pid_rate_xy: { kp: 0, ki: 0, imax: 0 },
    pid_rate_z: { kp: 0, ki: 0, imax: 0 },
    pid_angle_xy: { kp: 0, ki: 0, imax: 0 },
  }

  plainData = () => {
    return toJSON(this.data)
  }

  fetchConfig = () => {
    serial.send(serial.codes.REQUEST_CONFIG, null, (config) => {
      this.data = config
      this.fetchedOnce = true
    })
  }

  writeConfig = () => {
    if (!this.fetchedOnce) return 

    let plainData = this.plainData()
    plainData.version = CONFIG_VERSION
    const struct = buildStruct(plainData, structLayouts.config)

    serial.send(serial.codes.SET_CONFIG, struct)
  }
}

