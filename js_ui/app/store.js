import { observable, toJSON } from 'mobx'
import { deepFetch } from './utils'
import serial from './serial/serialManager'
import moment from 'moment'
import * as structLayouts from "./serial/structLayouts";
import { buildStruct } from './serial/struct'

class MetaStore {
  @observable currentTab = 'TUNING'
  @observable consoleMessages = []

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

  consoleMessage = (message) => {
    const time = moment().format("HH:mm:ss.SSS")
    this.consoleMessages.push({ text: message, time })

    while (this.consoleMessages.length > 30) this.consoleMessages.shift()
  }
}

export const metaStore = new MetaStore()

export const graphTypes = {
  'angleXFusion': ['accelAngles.x', 'gyroAngles.x', 'angles.x'],
  'angleYFusion': ['accelAngles.y', 'gyroAngles.y', 'angles.y']
}

const initialGraphData = function(type) {
  return { type, data }
}

class LineGraphStore {
  @observable data = []
  @observable type = 'angleXFusion'
  @observable paused = false
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
    if (this.paused) return

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

  pausedOrUnpaused = () => {
    this.paused = !this.paused
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
    plainData.version = 4
    const struct = buildStruct(plainData, structLayouts.config)

    serial.send(serial.codes.SET_CONFIG, struct)
  }
}

export const configStore = new ConfigStore()

class RCChannel {
  functions = [
    'Roll',
    'Pitch',
    'Throttle',
    'Yaw',
    'Pot A',
    'Pot B'
  ]

  @observable data

  constructor(data) {
    this.data = data
  }

  functionName() {
    return this.functions[this.data.function]
  }

  positiveBarStyle(value, min, max) {
    let percentage = this.positiveBarWidth(value, min, max)
    return({ width: `${percentage.toFixed(2)}%` })
  }

  positiveBarWidth(value, min, max) {
    let percentage = (value - min) / (max - min) * 100
    return percentage > 0 ? percentage : 0
  }

  rawBarStyle() {
    return this.positiveBarStyle(this.data.rawValue, 1000, 2000)
  }

  valueBarStyle() {
    if (this.data.valueMin > 0) {
      return this.positiveBarStyle(this.data.value, this.data.valueMin, this.data.valueMax)
    }

    if (this.data.value > 0) {
      let width = this.positiveBarWidth(this.data.value, 0, this.data.valueMax) / 2
      return { width: `${width.toFixed(2)}%`, left: '50%' }
    } else {
      let width = this.positiveBarWidth(-1 * this.data.value, 0,  -1 * this.data.valueMin) / 2
      return { width: `${width.toFixed(2)}%`, left: `${50 - width.toFixed(2)}%` }
    }

    // let percentage = (this.data.rawValue - 1000) / 10
    // percentage = percentage > 0 ? percentage : 0
    // return({ width: `${channel.rawBarWidth()}%` })

    return {}
  }
}

class RCTabStore {
  @observable channelData = []

  updateChannelData = (data) => {
    this.channelData = _.toArray(data).map((channel) => new RCChannel(channel))
  }
}

export const rcTabStore = new RCTabStore()
