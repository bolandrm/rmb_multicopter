import { observable } from 'mobx'
import { deepFetch } from '../utils'

export const graphTypes = {
  'angleXFusion': ['accelAngles.x', 'gyroAngles.x', 'angles.x'],
  'angleYFusion': ['accelAngles.y', 'gyroAngles.y', 'angles.y']
}

const initialGraphData = function(type) {
  return { type, data }
}

export class LineGraphStore {
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
