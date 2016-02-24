const Utils = {
  highByte: function(num) {
    return num >> 8;
  },

  lowByte: function(num) {
    return 0x00FF & num;
  },

  deepFetch: function(obj, path) {
    var current=obj; 
    path.split('.').forEach(function(p){ current = current[p]; }); 
    return current;
  },

  partialCombineReducers: function (mainReducer, reducers) {
    const combinedReducers = combineReducers(reducers)

    return function (state = {}, action) {
      return Object.assign(
        {},
        mainReducer(state, action),
        combinedReducers(state, action)
      )
    }
  }
};

export default Utils;
