import _ from 'lodash'
import { observable } from 'mobx'
import { LineGraphStore } from './LineGraphStore'

export default class TuningTabStore {
  @observable motorChartValues = { m1: 0, m2: 0, m3: 0, m4: 0 }

  constructor() {
    this.graph1 = new LineGraphStore()
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
}
