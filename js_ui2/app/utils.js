export const highByte = function(num) {
  return num >> 8
}

export const lowByte = function(num) {
  return 0x00FF & num
}

export const deepFetch = function(obj, path) {
  var current = obj
  path.split('.').forEach(function(p) { current = current[p] })
  return current
}

export const formValues = function formValues(formData) {
  const data = {}

  _.forOwn(formData, function(value, key) {
    if (key.match(/^_/)) return
    
    if (value.value) {
      data[key] = value.value
    } else {
      data[key] = formValues(value)
    }
  })

  return data
}
