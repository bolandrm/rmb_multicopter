import React, { Component } from 'react';
import { Link } from 'react-router';

export default class Home extends Component {
  render() {
    return (
      <div>
        <div>
          <h2>Home2</h2>
          <Link to="/counter">to Counter</Link>
        </div>
      </div>
    );
  }
}
