chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('index.html', {
    bounds: {
      width: 1500,
      height: 800
    },
    resizable: false
  });
});
