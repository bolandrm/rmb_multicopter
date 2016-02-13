$(function() {
  var timestamp = 500;
  var numberPoints = 500;
  var paused = false;

  var data1 = _.map(Array(500), function(n, i){ return {x: i, y: 0}; });
  var data2 = _.map(Array(500), function(n, i){ return {x: i, y: 0}; });
  var data3 = _.map(Array(500), function(n, i){ return {x: i, y: 0}; });
  var data4 = _.map(Array(500), function(n, i){ return {x: i, y: 0}; });

  $("#clear-line-button").click(function() {
    paused = true

    for (i = 0; i < 500; i++) {
      pushToLineChart([0, 0, 0, 0, 0, 0, 0, 0, 0, 0], true);
    }

    paused = false;
    console.log("resetting line chart");
  });

  $("#pause-button").click(function() {
    paused = !paused;

    if (paused) {
      $(this).text("Unpause")
    } else {
      $(this).text("Pause")
    }
  });

  var chart = new CanvasJS.Chart("line-chart", {
    toolTip: {
      enabled: false
    },
    legend: {
      cursor: "pointer",
      itemclick: function(e) {
        if (typeof(e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
          e.dataSeries.visible = false;
        } else {
          e.dataSeries.visible = true;
        }
        e.chart.render();
      }
    },
    axisX: {
      valueFormatString: " ",
      tickLength: 0
    },
    data: [
      {
        showInLegend: true,
        type: "line",
        dataPoints: data1
      },
      {
        showInLegend: true,
        type: "line",
        dataPoints: data2
      },
      {
        showInLegend: true,
        type: "line",
        dataPoints: data3
      },
      {
        showInLegend: true,
        type: "line",
        dataPoints: data4
      }
    ]
  });

  chart.render();

  var m1 = $('#m1-chart').epoch({
    type: 'time.gauge',
    value: 1000,
    domain: [1000, 1800],
    format: function(v) { return (Math.round(v)); }
  });
  var m2 = $('#m2-chart').epoch({
    type: 'time.gauge',
    value: 1000,
    domain: [1000, 1800],
    format: function(v) { return (Math.round(v)); }
  });
  var m3 = $('#m3-chart').epoch({
    type: 'time.gauge',
    value: 1000,
    domain: [1000, 1800],
    format: function(v) { return (Math.round(v)); }
  });
  var m4 = $('#m4-chart').epoch({
    type: 'time.gauge',
    value: 1000,
    domain: [1000, 1800],
    format: function(v) { return (Math.round(v)); }
  });

  pushToLineChart = function(data, force) {
    if (paused && !force) { return; }

    data1.push({
      x: timestamp,
      y: parseFloat(data[0])
    });

    data2.push({
      x: timestamp,
      y: parseFloat(data[1])
    });

    data3.push({
      x: timestamp,
      y: parseFloat(data[2])
    });

    data4.push({
      x: timestamp,
      y: parseFloat(data[3])
    });

    if (data1.length > numberPoints) {
      data1.shift();
      data2.shift();
      data3.shift();
      data4.shift();
    }

    chart.render();

    timestamp++;

    for (i = 4; i < 8; i++) {
      if (data[i] < 1000) {
        data[i] = 1001;
      }
    }

    m1.push(data[4]);
    m2.push(data[5]);
    m3.push(data[6]);
    m4.push(data[7]);
  };

});
