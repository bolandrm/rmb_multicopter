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
        serial.send(serial.codes.REQUEST_GYRO_ACC, null, tuningTabStore.updateOrientation)
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
    let flightData = tuningTabStore.flightData

    let orientation = tuningTabStore.orientation
    let orientationStyle = { transform: `rotateX(${orientation.x}deg) rotateY(${orientation.y}deg) rotateZ(${orientation.z}deg)` }

    return (
      <div className="tuning-tab">
        <div className="top-segment">
          <div className="fc-data">
            <div className={`flight-stat armed-status ${flightData.armed}`}>
              Arm status:
              <div className="callout-text uppercase">{flightData.armed}</div>
            </div>
            <div className="flight-stat">
              Mode:
              <div className="callout-text uppercase">{flightData.mode}</div>
            </div>
            <div className="flight-stat">
              Battery:
              <div className="callout-text">{flightData.batteryVoltage.toFixed(2)}v</div>
            </div>
            <div className="flight-stat">
              Loop Time:
              <div className="callout-text">
                {flightData.loopTime}μs
              </div>
            </div>
          </div>

          <div className="teapot" style={orientationStyle} >
            <div className="side front"></div>
            <div className="side back"></div>
            <div className="side left"></div>
            <div className="side right"></div>
            <div className="side top"></div>
            <div className="side bottom"></div>
          </div>
          <MotorChart data={tuningTabStore.motorChartValues} />
        </div>
        <LineGraph graph={tuningTabStore.graph1} />
      </div>
    )
  }
}

// orientation,
// calibration,
// reboot,
// test ping

export default TuningTab
