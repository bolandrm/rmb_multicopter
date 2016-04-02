import moment from 'moment'
import { observable } from 'mobx'

export default class MetaStore {
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
