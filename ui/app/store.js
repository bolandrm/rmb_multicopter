import MetaStore from './stores/MetaStore'
import TuningTabStore from './stores/TuningTabStore'
import ConfigStore from './stores/ConfigStore'
import RCTabStore from './stores/RCTabStore'

export const metaStore = new MetaStore()
export const tuningTabStore = new TuningTabStore()
export const configStore = new ConfigStore()
export const rcTabStore = new RCTabStore()
