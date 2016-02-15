import _ from "underscore";

const dataTypes = {
  //   b	  int8
  //   B	  uint8
  //   ?	  bool8
  h: { type: "Int16", byteSize: 2 },
  H: { type: "Uint16", byteSize: 2 },
  //   i	  int32
  //   I	  uint32
  //   q	  int64
  //   Q	  uint64
  f: { type: "Float32", byteSize: 4 },
  //   d	  double64
};

const parseStruct = function(dataView, structLayout, parsedStruct, i = 0) {
  structLayout.forEach(function(entry) {
    const field = entry[0];
    const type = entry[1];

    if (_.isArray(type)) {
      parsedStruct[field] = {};
      i = parseStruct(dataView, type, parsedStruct[field], i);
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

const buildStruct = function(dataView, structLayout, dataObject, i = 0) {
  structLayout.forEach(function(entry) {
    const field = entry[0];
    const type = entry[1];

    if (_.isArray(type)) {
      i = buildStruct(dataView, type, dataObject[field], i);
    } else {
      const method = `set${dataTypes[type].type}`;
      const value = dataObject[field];
      dataView[method](i, value, true);
      i += dataTypes[type].byteSize;
    }
  });

  return i;
};

const parse = function(buffer, structLayout) {
  const dataView = new DataView(buffer);
  const parsedStruct = {};

  parseStruct(dataView, structLayout, parsedStruct);

  return parsedStruct;
};

const build = function(dataObject, structLayout) {
  const buffer = new ArrayBuffer(structSize(structLayout));
  const dataView = new DataView(buffer);

  buildStruct(dataView, structLayout, dataObject);

  return buffer;
}

export default { parse: parse, build: build };
