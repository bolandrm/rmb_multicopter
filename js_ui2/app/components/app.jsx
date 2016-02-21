import React from "react"
import ToolbarContainer from "./toolbar_container"
import TabContentContainer from "./tab_content_container"

class App extends React.Component {
  render() {
    return(
      <div>
        <ToolbarContainer />
        <TabContentContainer />
      </div>
    )
  }
}

export default App
