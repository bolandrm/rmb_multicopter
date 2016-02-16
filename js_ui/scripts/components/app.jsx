import React from "react";
import Toolbar from "./toolbar";
import TuningTab from "./tuning_tab";

class App extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      connected: false
    };

    serial.addConnectionStatusListener(this.connectionChanged)
  }

  connectionChanged = (status) => {
    if (status === "connected") {
      this.setState({connected: true});
    } else if (status === "disconnected") {
      this.setState({connected: false});
    }
  }

  render() {
    return(
      <div>
        <Toolbar connected={this.state.connected} />
        <TuningTab />
      </div>
    );
  }
}

export default App;
