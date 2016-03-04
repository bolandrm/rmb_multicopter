import { observable, toJSON } from 'mobx'
import { deepFetch } from './utils'
import serial from './serialManager'
import SerialCodes from './serial_codes'
import StructLayouts from './struct_layouts'
import Struct from './struct'

class MetaStore {
  @observable currentTab = 'TUNING';

  tabData = [
    { key: 'TUNING', text: 'Tuning' },
    { key: 'PIDS', text: 'PIDs' },
    { key: 'RC', text: 'Remove Control' },
    { key: 'IMU', text: 'IMU' },
    { key: 'MISC', text: 'Misc' },
  ]

  tabSelected(tab) {
    this.currentTab = tab
  }
}

export const metaStore = new MetaStore()

const graphTypes = {
  'angleXFusion': ['accelAngles.x', 'gyroAngles.x', 'angles.x'],
  'angleYFusion': ['accelAngles.y', 'gyroAngles.y', 'angles.y']
}

const initialGraphData = function(type) {
  return { type, data }
}

class LineGraphStore {
  @observable data = []
  @observable type = 'angleXFusion'
  sampleCount = 200

  constructor() {
    this.seedData(this.type)
  }

  seedData(type) {
    graphTypes[type].forEach((key, i) => {
      this.data.push({ key: key, samples: [], filtered: false })
    })
  }

  addSample = (sample) => {
    const shift = (this.data[0].samples.length >= this.sampleCount)

    const data = this.data.forEach((series, i) => {
      series.samples.push(deepFetch(sample, series.key))
      if (shift) series.samples.shift()
    })
  }

  graphFiltered(key) {
    if (!this.data.find((s) => s.key === key).filtered &&
        this.data.filter((s) => !s.filtered).length === 1)
      return

    this.data.forEach((s) => {
      const filtered = (s.key === key ? !s.filtered : s.filtered)
      s.filtered = filtered
    })
  }
}

class TuningTabStore {
  constructor() {
    this.graph1 = new LineGraphStore()
  }
}

export const tuningTabStore = new TuningTabStore()

class ConfigStore {
  @observable fetchedOnce = false
  @observable data = {
    pid_rate_xy: { kp: 0, ki: 0, imax: 0 },
    pid_rate_z: { kp: 0, ki: 0, imax: 0 },
    pid_angle_xy: { kp: 0, ki: 0, imax: 0 },
  }

  fetchConfig = () => {
    serial.send(SerialCodes.REQUEST_CONFIG, null, (config) => {
      this.data = config
      this.fetchedOnce = true
    })
  }

  writeConfig = () => {
    if (!this.fetchedOnce) return 

    let plainData = toJSON(this.data)
    plainData.version = 3
    const struct = Struct.build(plainData, StructLayouts.config)

    serial.send(SerialCodes.SET_CONFIG, struct)
  }
}

export const configStore = new ConfigStore()
