import React from "react";
import { observer } from 'mobx-react'
import LineGraph from './LineGraph'
import MotorChart from './MotorChart'
import { tuningTabStore } from '../store'
import serial from '../serial/serialManager'

@observer
class TuningTab extends React.Component {
  componentDidMount() {
    this.intervalIDs = [
      setInterval(() => {
        serial.send(tuningTabStore.graph1.requestCode(), null, tuningTabStore.graph1.addSample)
        serial.send(serial.codes.REQUEST_MOTORS, null, tuningTabStore.addMotorChartSample)
      }, 50),

      setInterval(() => {
        serial.send(serial.codes.REQUEST_FLIGHT_DATA, null, tuningTabStore.updateFlightControllerData)
      }, 200),
    ]
  }

  componentWillUnmount() {
    this.intervalIDs.forEach((id) => clearInterval(id))
  }

  render() {
    return (
      <div>
        <LineGraph graph={tuningTabStore.graph1} />
        <MotorChart data={tuningTabStore.motorChartValues} />
        <div>
          Mode: {tuningTabStore.flightData.mode}
          <br />
          {tuningTabStore.flightData.armed}
          <br />
          Battery: {tuningTabStore.flightData.batteryVoltage.toFixed(2)}v
          <br />
          Loop Time: {tuningTabStore.flightData.loopTime}μs
        </div>
        orientation,
        calibration,
        reboot,
        test ping
      </div>
    )
  }
}

export default TuningTab
