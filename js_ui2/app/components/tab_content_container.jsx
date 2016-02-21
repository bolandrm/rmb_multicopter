import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import * as actions from '../actions'
import TuningTab from "./tuning_tab"

class TabContentContainer extends React.Component {
  render() {
    return (
      <div>
        {this.props.currentTab === 'GRAPHS' &&
          <TuningTab />
        }
      </div>
    )
  }
}

export default connect(
  (state) => {
    return {
      currentTab: state.meta.currentTab
    }
  },

  (dispatch) => {
    return { actions: bindActionCreators(actions, dispatch) }
  }
)(TabContentContainer)
