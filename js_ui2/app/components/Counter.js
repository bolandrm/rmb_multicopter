import React, { Component, PropTypes } from 'react';
import { Link } from 'react-router';

class Counter extends Component {
  static propTypes = {
    increment: PropTypes.func.isRequired,
    incrementIfOdd: PropTypes.func.isRequired,
    incrementAsync: PropTypes.func.isRequired,
    decrement: PropTypes.func.isRequired,
    counter: PropTypes.number.isRequired
  };

  render() {
    const { increment, incrementIfOdd, incrementAsync, decrement, counter } = this.props;
    return (
      <div>
        <div>
          <Link to="/">
            <i className="fa fa-arrow-left fa-3x" />
          </Link>
        </div>
        <div className={`counter`}>
          {counter}
        </div>
        <div>
          <button onClick={increment}>
            <i className="fa fa-plus"></i>
          </button>
          <button onClick={decrement}>
            <i className="fa fa-minus"></i>
          </button>
          <button onClick={incrementIfOdd}>odd</button>
          <button onClick={() => incrementAsync()}>async</button>
        </div>
      </div>
    );
  }
}

export default Counter;
