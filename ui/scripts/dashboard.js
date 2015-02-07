$(function() {
  var timestamp = 500;
  var numberPoints = 500;

  var data1 = _.map(Array(500), function(n, i){ return {x: i, y: 0}; });
  var data2 = _.map(Array(500), function(n, i){ return {x: i, y: 0}; });

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
      }
    ]
  });

  //var areaChartInstance = $('#area').epoch({
  //  type: 'time.line',
  //  data: data,
  //  axes: ['left'],
  //  windowSize: 500,
  //  fps: 60
  //});

  pushToLineChart = function(data) {
    data1.push({
      x: timestamp,
      y: parseFloat(data[0])
    });

    data2.push({
      x: timestamp,
      y: parseFloat(data[1])
    });

    if (data1.length > numberPoints) {
      data1.shift();
      data2.shift();
    }

    chart.render();

    timestamp++;
  };

  // for(i = 0; i < 15; i++) {
  //   areaChartInstance.push([
  //     { timestamp: timestamp, y: 30+timestamp },
  //     { timestamp: timestamp, y: 30-timestamp }
  //   ]);

  //   timestamp++;
  // }
});
