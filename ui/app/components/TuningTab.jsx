import React from "react";
import { observer } from 'mobx-react'
import LineGraph from './LineGraph'
import MotorChart from './MotorChart'
import { tuningTabStore } from '../store'
import serial from '../serial/serialManager'

@observer
class TuningTab extends React.Component {
  componentDidMount() {
    this.intervalID = setInterval(this.fetchData, 50)
  }

  componentWillUnmount() {
    clearInterval(this.intervalID)
  }

  fetchData = () => {
    serial.send(tuningTabStore.graph1.requestCode(), null, tuningTabStore.graph1.addSample)
    serial.send(serial.codes.REQUEST_MOTORS, null, tuningTabStore.addMotorChartSample)
  }

  render() {
    return (
      <div>
        <LineGraph graph={tuningTabStore.graph1} />
        <MotorChart data={tuningTabStore.motorChartValues} />
      </div>
    )
  }
}

export default TuningTab
