chrome.app.runtime.onLaunched.addListener(function() {
  var windowWidth = 1500;
  var windowHeight = 800;

  chrome.app.window.create('index.html', {
    outerBounds: {
      width: windowWidth,
      height: windowHeight,
      left: screen.availWidth - windowWidth,
      top: screen.availHeight - windowHeight
    },
    resizable: false
  });
});

// chrome.commands.onCommand.addListener(function(command) {
//   console.log('Command:', command);
// });
// 
