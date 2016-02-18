import React from "react"
import ToolbarContainer from "./toolbar_container"
import TuningTab from "./tuning_tab"

class App extends React.Component {
  render() {
    return(
      <div>
        <ToolbarContainer />
        <TuningTab />
      </div>
    )
  }
}

export default App
