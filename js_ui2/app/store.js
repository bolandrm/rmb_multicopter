import { observable, autorun } from 'mobx'
import { deepFetch } from './utils'

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
