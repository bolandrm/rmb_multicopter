import React from "react";
import ReactDOM from "react-dom";
import d3 from "d3";

class TuningTab extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
    };
  }

  componentDidMount() {
    var el = ReactDOM.findDOMNode(this);

    var data = [
      { key: "jaja1", samples: [3, 6, 2, 7, 5, 2, 0, 3, 8, 9, 2, 5, 9, 3, 6, 3, 6, 2, 7, 5, 2, 1, 3, 8, 9, 2, 5, 9, 2, 7] },
      { key: "yaya2", samples: [5, 7, 3, 4, 6, 0, 9, 4, 11, 7, 4, 1, 9, 3, 6, 3, 6, 2, 7, 5, 2, 1, 3, 8, 9, 2, 5, 9, 2, 7] }
    ];

    var s = [3, 6, 2, 7, 5, 2, 0, 3, 8, 9, 2, 5, 9, 3, 6, 3, 6, 2, 7, 5, 2, 1, 3, 8, 9, 2, 5, 9, 2, 7];

    var margin = {top: 20, right: 20, bottom: 30, left: 50};
    var width = el.offsetWidth - margin.left - margin.right;
    var height = 300;

    var max = d3.max(data, function(d) { return d3.max(d.samples) });
    var min = d3.min(data, function(d) { return d3.min(d.samples) });

    console.log(max);
    console.log(min);

		var x = d3.scale.linear().domain([0, data[0].samples.length]).range([0, width]);
    var y = d3.scale.linear().domain([min - 5, max + 5]).range([height, 0]);

    var c10 = d3.scale.category10();

		var line = d3.svg.line()
			.x(function(d, i) { return x(i); })
			.y(function(d) { return y(d); })

			var graph = d3.select(el).append("svg")
			      .attr("width", width + margin.left + margin.right)
			      .attr("height", height + margin.top + margin.bottom)
            .append("g")
            .attr("transform", `translate(${margin.left},${margin.top})`);

			var xAxis = d3.svg.axis().scale(x).orient("bottom");
      var yAxis = d3.svg.axis().scale(y).orient("left");

			graph.append("g")
			      .attr("class", "x axis")
            .attr("transform", `translate(0,${height})`)

      graph.append("g").attr("class", "y axis")

      graph.select(".x.axis").call(xAxis);
      graph.select(".y.axis").call(yAxis);

      graph.append("g").attr("class", "data");

      graph.select("g.data")
           .selectAll("path")
           .data(data, function(d) { return d.key; })
           .enter()
           .append("path")
           .attr("class", "line")
           .attr("d", function(d) { return line(d.samples); })
           .style("stroke", function(d) { return c10(d.key); });

           //graph.append("text")
           //    .attr("x", function(d, i){})
           //    .attr("y", height + (margin.bottom/2) + 5)
           //    .attr("class", "legend")
           //    .style("fill", function() {
           //        return d.color = color(d.key); })
           //    .text(d.key);

     // d3.select(el).select("svg").remove();
  }

  render() {
    return(
      <div className="line-chart">
      </div>
    );
  }
}

export default TuningTab;
