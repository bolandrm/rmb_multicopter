$(function() {
  $("#connect-button").click(function() {
    if (connectionId !== -1) {
      chrome.serial.disconnect(connectionId, function(){});
      connectionId = -1;
    } else {
      openSelectedPort();
    }
  });

  var connectionId = -1;
  var readBuffer = "";

  ab2str = function(buf) {
    return String.fromCharCode.apply(null, new Uint8Array(buf));
  }

  var handleData = function(info) {
    readBuffer += ab2str(info.data);

    if (readBuffer.match(/\n/)) {
      pushToLineChart(readBuffer.split(" "));
      console.log(readBuffer);
      readBuffer = "";
    }
  };

  var openSelectedPort = function() {
    var port = $("#port-picker").val();
    chrome.serial.connect(port, { bitrate: 115200 }, function(connectionInfo) {
      connectionId = connectionInfo.connectionId;

      if (connectionId === -1) {
        console.log("could not open");
      } else {
        console.log("opened port");
      }
    });
  }

  var buildPortPicker = function(ports) {
    ports = _.map(ports, function(port) {
      return port.path;
    });

    console.log(ports);

    ports = _.filter(ports, function(port) {
      return !port.match(/[Bb]luetooth/) && port.match(/\/dev\/tty/);
    });

    var portPicker = $('#port-picker');

    _.each(ports, function(port) {
      var portOption = $("<option></option>");
      portOption.val(port);
      portOption.text(port);
      portPicker.append(portOption);
    });

    portPicker.change(function() {
      if (connectionId != -1) {
        chrome.serial.disconnect(connectionId, openSelectedPort);
      } else {
        openSelectedPort();
      }
    });
  }

  onload = function() {
    chrome.serial.getDevices(buildPortPicker);
    chrome.serial.onReceive.addListener(handleData);
  };
});
