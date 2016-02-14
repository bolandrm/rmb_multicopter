cfg.Utils = {
  highByte: function(num) {
    return num >> 8;
  },

  lowByte: function(num) {
    return 0x00FF & num;
  }
};
