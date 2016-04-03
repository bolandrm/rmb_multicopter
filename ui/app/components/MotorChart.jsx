import _ from 'lodash'
import React from 'react'
import ReactDOM from 'react-dom'
import { observer } from 'mobx-react'
import { toJSON } from 'mobx'


@observer
class MotorChart extends React.Component {
  render() {
    const data = toJSON(this.props.data)
    const motorPositions = ['fr', 'bl', 'fl', 'br']

    return (
      <div className='motor-chart-2d'>
        {_.values(data).map((value, i) => {
          return(
            <svg key={i} className={motorPositions[i]} viewBox='0 0 32 32'>
              <circle r='16' cx='16' cy='16' style={{'strokeDasharray': `${value} 100`}} />
            </svg>
          )
        })}

        <div className='arm1'></div>
        <div className='arm2'></div>
      </div>
    )
  }
}

export default MotorChart
