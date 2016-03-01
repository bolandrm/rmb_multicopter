import React from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'

class Console extends React.Component {
  render() {
    const { messages } = this.props

    return (
      <div className='console well'>
        {messages.map((message, i) => {
          return (
            <div key={i} className='console-entry'>
              <span className='console-time'>{message.time}:</span> {message.text}
            </div>
          )
        })}
      </div>
    )
  }
}

export default connect(
  (state) => {
    return {
      messages: state.meta.consoleMessages
    }
  },

  (dispatch) => { return {} }
)(Console)
