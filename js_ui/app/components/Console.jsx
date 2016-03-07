import React from 'react'
import { observer } from 'mobx-react'
import { metaStore } from '../store'

@observer
class Console extends React.Component {
  render() {
    return (
      <div className='console well'>
        {metaStore.consoleMessages.reverse().map((message, i) => {
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

export default Console
