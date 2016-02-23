import React from "react";
import ReactDOM from "react-dom";
import d3 from "d3";

class LineGraph extends React.Component {
  SAMPLE_COUNT = 200

  constructor(props) {
    console.log(props)
    super(props);
  }

  componentDidMount() {
    this.createChart();
  }

  componentWillReceiveProps(nextProps) {
    this.updateChart()
  }

  shouldComponentUpdate() {
    return false;
  }

  createChart() {
    this.el = ReactDOM.findDOMNode(this);

    this.margin = {top: 20, right: 20, bottom: 30, left: 50};
    this.width = this.el.offsetWidth - this.margin.left - this.margin.right;
    this.height = 250;

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

  legendClick = (series) => {
    this.props.actions.graphFiltered({ graph: this.props.graph, key: series.key})
  }

  updateChart() {
    const data = this.props.data
    const filtered_data = this.props.data.filter((d) => !d.filtered)

    var max = d3.max(filtered_data, function(d) { return d3.max(d.samples) });
    var min = d3.min(filtered_data, function(d) { return d3.min(d.samples) });

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
         .data(data, function(d) { return d.key; })

    lines.enter()
         .append("path")
         .attr("class", "line")
         .style("stroke", (d) => { return this.color(d.key); });

    lines.attr("d", function(d) { return line(d.samples); })
         .style("opacity", (d) => (d.filtered ? 0 : 1))

    var legend = this.svg.select("g.legend").selectAll('g.legend-entry')
        .data(data, function(d) { return d.key })
        .enter()
      .append('g')
        .on('click', this.legendClick)
        .attr('class', 'legend-entry')
      
    legend.append('rect')
        .attr('x', this.width - 62)
        .attr('y', function(d, i){ return i *  20;})
        .attr('width', 10)
        .attr('height', 10)
        .style('fill', (d) => { return this.color(d.key); });
        
    legend.append('text')
        .attr('x', this.width - 50)
        .attr('y', function(d, i){ return (i *  20) + 9;})
        .text(function(d){ return d.key; })

     // d3.select(el).select("svg").remove();
  }

  render() {
    return <div className="line-chart" />
  }
}

export default LineGraph;
