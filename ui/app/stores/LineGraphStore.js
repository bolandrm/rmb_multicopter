import { observable } from 'mobx'
import { deepFetch } from '../utils'
import * as serialCodes from '../serial/serialCodes'

export const graphTypes = {
  'angleXFusion': {
    requestCode: serialCodes.REQUEST_GYRO_ACC,
    dataPoints: ['accelAngles.x', 'gyroAngles.x', 'angles.x']
  },
  'angleYFusion': {
    requestCode: serialCodes.REQUEST_GYRO_ACC,
    dataPoints: ['accelAngles.y', 'gyroAngles.y', 'angles.y']
  },
  'motors': {
    requestCode: serialCodes.REQUEST_MOTORS,
    dataPoints: ['m1', 'm2', 'm3', 'm4']
  }
}

export class LineGraphStore {
  @observable data = []
  @observable type = 'angleXFusion'
  @observable paused = false
  sampleCount = 200

  constructor() {
    this.seedData()
  }

  seedData() {
    graphTypes[this.type].dataPoints.forEach((key, i) => {
      this.data.push({ key: key, samples: [], filtered: false })
    })
  }

  requestCode = () => {
    return graphTypes[this.type].requestCode
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
