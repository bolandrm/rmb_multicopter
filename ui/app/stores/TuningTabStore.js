import _ from 'lodash'
import { observable } from 'mobx'
import { LineGraphStore } from './LineGraphStore'

export default class TuningTabStore {
  @observable motorChartValues = { m1: 0, m2: 0, m3: 0, m4: 0 }
  @observable orientation = { x: 0, y: 0, z: 0 }
  @observable flightData = {}

  constructor() {
    this.graph1 = new LineGraphStore()
    this.flightData = { batteryVoltage: 0.0, armed: "unknown" }
  }

  addMotorChartSample = (sample) => {
    const motorSampleMin = 1060
    const motorSampleMax = 1860

    _.forOwn(sample, (value, key) => {
      if (value < motorSampleMin) value = motorSampleMin
      else if (value > motorSampleMax) value = motorSampleMax

      value = (value - motorSampleMin) / (motorSampleMax - motorSampleMin) * 100

      this.motorChartValues[key] = value
    })
  }

  updateOrientation = (data) => {
    this.orientation.x = data.angles.y
    this.orientation.y = 0
    this.orientation.z = data.angles.x
  }

  updateFlightControllerData = (data) => {
    this.flightData = data
  }
}
