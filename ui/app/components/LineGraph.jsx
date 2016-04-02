import React from "react"
import ReactDOM from "react-dom"
import { observer } from 'mobx-react'
import { autorun } from 'mobx'
import d3 from "d3"

@observer
class LineGraph extends React.Component {
  componentDidMount() {
    this.createChart()
    this.stopUpdating = autorun(this.updateChart)
  }

  componentWillUnmount() {
    this.stopUpdating()
  }

  createChart() {
    const el = ReactDOM.findDOMNode(this)

    this.margin = {top: 20, right: 20, bottom: 30, left: 50}
    this.width = 1450 - this.margin.left - this.margin.right
    this.height = 250

    this.svg = d3.select(el).select(".line-chart").append("svg")
          .attr("width", this.width + this.margin.left + this.margin.right)
          .attr("height", this.height + this.margin.top + this.margin.bottom)
          .append("g")
          .attr("transform", `translate(${this.margin.left},${this.margin.top})`)

    this.color = d3.scale.category10()

    this.svg.append("g")
          .attr("class", "x axis")
          .attr("transform", `translate(0,${this.height})`)

    this.svg.append("g").attr("class", "y axis")
    this.svg.append("g").attr("class", "data")
    this.svg.append("g").attr("class", "legend")

    this.updateChart()
  }

  legendClick = (series) => {
    this.props.graph.graphFiltered(series.key)
    this.updateChart()
  }

  updateChart = () => {
    const data = this.props.graph.data
    const filteredData = data.filter((d) => !d.filtered)

    var max = d3.max(filteredData, (d) => d3.max(d.samples))
    var min = d3.min(filteredData, (d) => d3.min(d.samples))

		var x = d3.scale.linear().domain([0, this.props.graph.sampleCount]).range([0, this.width])
    var y = d3.scale.linear().domain([min - 5, max + 5]).range([this.height, 0])

		var line = d3.svg.line()
      .x((d, i) => x(i))
      .y((d) => y(d))

    var xAxis = d3.svg.axis().scale(x).orient("bottom")
    var yAxis = d3.svg.axis().scale(y).orient("left")

    this.svg.select(".x.axis").call(xAxis)
    this.svg.select(".y.axis").call(yAxis)

    var lines = this.svg.select("g.data")
         .selectAll("path")
         .data(data, (d) => d.key)

    lines.enter()
         .append("path")
         .attr("class", "line")
         .style("stroke", (d) => this.color(d.key))

    lines.attr("d", (d) => line(d.samples))
         .style("opacity", (d) => (d.filtered ? 0 : 1))

    var legendEntries = this.svg.select("g.legend")
          .selectAll('g.legend-entry')

    var newEntries = legendEntries
          .data(data, (d) => d.key)
          .enter()
          .append('g')
            .on('click', this.legendClick)
            .attr('class', 'legend-entry')

    legendEntries.style('opacity', (d) => (d.filtered ? 0.5 : 1))

    newEntries.append('rect')
      .attr('x', this.width - 92)
      .attr('y', (d, i) => i *  20)
      .attr('width', 10)
      .attr('height', 10)
      .style('fill', (d) => this.color(d.key))

    newEntries.append('text')
      .attr('x', this.width - 80)
      .attr('y', (d, i) => (i * 20) + 9)
      .text((d) => d.key)
  }

  render() {
    const graph = this.props.graph

    return (
      <div>
        <div className="line-chart" />
        <a className="btn btn-default" onClick={graph.pausedOrUnpaused}>
          { graph.paused ? "Unpause" : "Pause" }
        </a>
      </div>
    )
  }
}

export default LineGraph
