import React from "react";
import { observer } from 'mobx-react'
import LineGraph from './LineGraph'
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
    serial.send(serial.codes.REQUEST_GYRO_ACC, null, tuningTabStore.graph1.addSample)
  }

  render() {
    return (
      <div>
        <LineGraph graph={tuningTabStore.graph1} />
      </div>
    )
  }
}

export default TuningTab
