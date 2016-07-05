import _ from 'lodash';

const dataTypes = {
  //   b	  int8
  uint8: { type: 'Uint8', byteSize: 1 },
  //   ?	  bool8
  int16: { type: 'Int16', byteSize: 2 },
  uint16: { type: 'Uint16', byteSize: 2 },
  int32: { type: 'Int32', byteSize: 4 },
  uint32: { type: 'Uint32', byteSize: 4 },
  //   q	  int64
  //   Q	  uint64
  f32: { type: 'Float32', byteSize: 4 },
  //   d	  double64
};

const _parseStruct = function(dataView, structLayout, parsedStruct, i = 0) {
  structLayout.forEach(function(entry) {
    const field = entry[0];
    const type = entry[1];

    if (_.isArray(type)) {
      parsedStruct[field] = {};
      i = _parseStruct(dataView, type, parsedStruct[field], i);
    } else {
      const method = `get${dataTypes[type].type}`;
      parsedStruct[field] = dataView[method](i, true);
      i += dataTypes[type].byteSize;
    }
  });

  return i;
}

const structSize = function(structLayout) {
  let size = 0;

  structLayout.forEach(function(entry) {
    const field = entry[0];
    const type = entry[1];

    if (_.isArray(type)) {
      size += structSize(type);
    } else {
      size += dataTypes[type].byteSize;
    }
  });

  return size;
};

const _buildStruct = function(dataView, structLayout, dataObject, i = 0) {
  structLayout.forEach(function(entry) {
    const field = entry[0];
    const type = entry[1];

    if (_.isArray(type)) {
      i = _buildStruct(dataView, type, dataObject[field], i);
    } else {
      const method = `set${dataTypes[type].type}`;
      const value = dataObject[field];
      dataView[method](i, value, true);
      i += dataTypes[type].byteSize;
    }
  });

  return i;
};

export const parseStruct = function(buffer, structLayout) {
  const dataView = new DataView(buffer);
  const parsedStruct = {};

  _parseStruct(dataView, structLayout, parsedStruct);

  return parsedStruct;
};

export const buildStruct = function(dataObject, structLayout) {
  const buffer = new ArrayBuffer(structSize(structLayout));
  const dataView = new DataView(buffer);

  _buildStruct(dataView, structLayout, dataObject);

  return buffer;
}
