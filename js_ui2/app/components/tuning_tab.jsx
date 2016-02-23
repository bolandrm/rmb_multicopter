import React from "react";
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as actions from '../actions'
import LineGraph from './line_graph'

class TuningTab extends React.Component {
  render() {
    return (
      <div>
        <LineGraph {...this.props.graph1} actions={this.props.actions} graph='graph1' />
      </div>
    )
  }
}

export default connect(
  (state) => {
    const graphs = state.tabs.graphs

    return {
      graph1: { ...graphs.graph1 }
    }
  },

  (dispatch) => {
    return { actions: bindActionCreators(actions, dispatch) }
  }
)(TuningTab)
