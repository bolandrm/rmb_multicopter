import React from "react";
import * as actions from '../actions'
import LineGraph from './LineGraph'

class TuningTab extends React.Component {
  render() {
    return (
      <div>
        <LineGraph {...this.props.graph1} actions={this.props.actions} graph='graph1' />
      </div>
    )
  }
}

export default TuningTab
