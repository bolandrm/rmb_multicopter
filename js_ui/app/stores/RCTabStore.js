import { observable } from 'mobx'

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
  }
}

export default class RCTabStore {
  @observable channelData = []

  updateChannelData = (data) => {
    this.channelData = _.toArray(data).map((channel) => new RCChannel(channel))
  }
}
