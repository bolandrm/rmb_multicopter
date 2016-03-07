import React from 'react'
import Toolbar from './Toolbar'
import TabContentContainer from './TabContentContainer'
import Console from './Console'

class App extends React.Component {
  render() {
    return(
      <div>
        <Toolbar />
        <TabContentContainer />
        <Console />
      </div>
    )
  }
}

export default App
