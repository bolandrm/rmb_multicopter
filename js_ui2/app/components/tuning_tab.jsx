import Utils from "../utils";
import React from "react";
import ReactDOM from "react-dom";
import d3 from "d3";
import serial from "../serial_manager";
import SerialCodes from "../serial_codes";

class TuningTab extends React.Component {
  SAMPLE_COUNT = 200;

  constructor(props) {
    super(props);

    this.state = {};

    this.keys = ["accelAngles.x", "gyroAngles.x", "angles.x"]

    this.data = this.keys.map(function(key) {
      return { key: key, samples: [] };
    });
  }

  componentDidMount() {
    this.createChart();
    this.timer = setInterval(this.requestData, 50);
  }

  appendData = (newData) => {
    if (this.data[0].samples.length >= this.SAMPLE_COUNT-1) {
      var shift = true;
    } else {
      var shift = false;
    }

    this.data.forEach(function(series, i) {
      series.samples.push(Utils.deepFetch(newData, series.key));
      if (shift) { series.samples.shift() };
    });

    this.updateChart();
  }

  requestData = () => {
    serial.send(SerialCodes.REQUEST_GYRO_ACC, null, this.appendData);
  }

  createChart() {
    this.el = ReactDOM.findDOMNode(this);

    this.margin = {top: 20, right: 20, bottom: 30, left: 50};
    this.width = this.el.offsetWidth - this.margin.left - this.margin.right;
    this.height = 300;

    this.svg = d3.select(this.el).append("svg")
          .attr("width", this.width + this.margin.left + this.margin.right)
          .attr("height", this.height + this.margin.top + this.margin.bottom)
          .append("g")
          .attr("transform", `translate(${this.margin.left},${this.margin.top})`);

    this.color = d3.scale.category10();

    this.svg.append("g")
          .attr("class", "x axis")
          .attr("transform", `translate(0,${this.height})`)

    this.svg.append("g").attr("class", "y axis")
    this.svg.append("g").attr("class", "data");
    this.svg.append("g").attr("class", "legend");

    this.updateChart();
  }

  updateChart() {
    var max = d3.max(this.data, function(d) { return d3.max(d.samples) });
    var min = d3.min(this.data, function(d) { return d3.min(d.samples) });

		var x = d3.scale.linear().domain([0, this.SAMPLE_COUNT]).range([0, this.width]);
    var y = d3.scale.linear().domain([min - 5, max + 5]).range([this.height, 0]);

		var line = d3.svg.line()
			.x(function(d, i) { return x(i); })
			.y(function(d) { return y(d); })

    var xAxis = d3.svg.axis().scale(x).orient("bottom");
    var yAxis = d3.svg.axis().scale(y).orient("left");

    this.svg.select(".x.axis").call(xAxis);
    this.svg.select(".y.axis").call(yAxis);

    var lines = this.svg.select("g.data")
         .selectAll("path")
         .data(this.data, function(d) { return d.key; })

    lines.enter()
         .append("path")
         .attr("class", "line")
         .style("stroke", (d) => { return this.color(d.key); });

    lines.attr("d", function(d) { return line(d.samples); })

    var legend = this.svg.select("g.legend").selectAll('g.legend-entry')
        .data(this.data, function(d) { return d.key })
        .enter()
      .append('g')
        .attr('class', 'legend-entry');
      
    legend.append('rect')
        .attr('x', this.width - 62)
        .attr('y', function(d, i){ return i *  20;})
        .attr('width', 10)
        .attr('height', 10)
        .style('fill', (d) => { return this.color(d.key); });
        
    legend.append('text')
        .attr('x', this.width - 50)
        .attr('y', function(d, i){ return (i *  20) + 9;})
        .text(function(d){ return d.key; });

     // d3.select(el).select("svg").remove();
  }

  render() {
    return <div className="line-chart" />;
  }
}

export default TuningTab;
