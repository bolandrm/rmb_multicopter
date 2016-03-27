import React from "react";
import { observer } from 'mobx-react'
import { rcTabStore } from '../store'
import serial from '../serial/serialManager'

@observer
class TuningTab extends React.Component {
  componentDidMount() {
    this.intervalID = setInterval(this.fetchData, 25)
  }

  componentWillUnmount() {
    clearInterval(this.intervalID)
  }

  fetchData = () => {
    serial.send(serial.codes.REQUEST_RC, null, rcTabStore.updateChannelData)
  }

  render() {
    return (
      <div className="rc-tab">
        <table className="table">
          <tbody>
            <tr>
              <th>Channel</th>
              <th>Function</th>
              <th>Raw Value</th>
              <th>Computed Value</th>
              <th>Endpoints</th>
            </tr>

            {rcTabStore.channelData.map((channel, i) => {
              return(
                <tr key={i}>
                  <td className="channel-col">{i + 1}</td>
                  <td className="function-col">{channel.functionName()}</td>
                  <td className="raw-value-col">
                    <div className="progress">
                      <div className="progress-bar" style={channel.rawBarStyle()}></div>
                    </div>
                    {channel.data.rawValue}
                  </td>
                  <td className="value-col">
                    <div className="progress">
                      {channel.data.valueMin < 0 &&
                        <div className='progress-bar-divider' />
                      }
                      <div className="progress-bar" style={channel.valueBarStyle()}></div>
                    </div>
                    {channel.data.value.toFixed(2)}
                  </td>
                  <td className="endpoints-col">
                    [{channel.data.valueMin}, {channel.data.valueMax}]
                  </td>
                </tr>
              )
            })}
          </tbody>



        </table>
      </div>
    )
  }
}

export default TuningTab
