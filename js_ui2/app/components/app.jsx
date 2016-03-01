import React from "react"
import ToolbarContainer from "./toolbar_container"
import TabContentContainer from "./TabContentContainer"
import Console from "./Console"

class App extends React.Component {
  render() {
    return(
      <div>
        <ToolbarContainer />
        <TabContentContainer />
        <Console />
      </div>
    )
  }
}

export default App
