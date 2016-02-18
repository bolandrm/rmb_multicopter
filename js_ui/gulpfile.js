var gulp = require('gulp');
var sourcemaps = require('gulp-sourcemaps');
var source = require('vinyl-source-stream');
var buffer = require('vinyl-buffer');
var browserify = require('browserify');
var watchify = require('watchify');
var babelify = require('babelify');
var tinylr = require('tiny-lr');
var exec = require('child_process').exec;
var concat = require('gulp-concat');

function compile(watch) {
  var bundler = browserify('./scripts/init.jsx', { debug: true, extensions: [".jsx"] })
    .transform(babelify, {
      presets: ['es2015', 'react'],
      plugins: ["transform-class-properties", "transform-object-rest-spread"]
    })

  if (watch) {
    bundler = watchify(bundler);
  }

  function rebundle() {
    bundler.bundle()
      .on("error", function(err) {
        console.log("Browserify error:", err);
        this.emit('end');
      })
      .pipe(source('app.js'))
      .pipe(buffer())
      .pipe(sourcemaps.init({ loadMaps: true }))
      .pipe(sourcemaps.write('./'))
      .pipe(gulp.dest('./dist'))
  }

  if (watch) {
    bundler.on('update', function() {
      console.log('-> bundling...');
      rebundle();
    });
  }

  rebundle();
}

function watch() {
  return compile(true);
};

gulp.task('watch', function () {
  // var lr = tinylr();
  // lr.listen(35729);

  // gulp.watch(['**.{css,html}', './.dist/*.js'], function (e) {
  //   console.log("reloading");

  //   lr.changed({
  //     body: {
  //       files: [e.path]
  //     }
  //   });
  // });

  compile();
  watch();

  gulp.start('launch');
});

gulp.task('build', function() { return compile(); });

gulp.task('launch', function (cb) {
  var launchApp = '`/Applications/Google\\ Chrome.app/Contents/MacOS/Google\\ Chrome --profile-directory=Default --app-id=alihedogaebgomfcknhkbiikjocomeae`';
  exec(launchApp, function (err, stdout, stderr) {
    console.log(stdout);
    console.log(stderr);
    cb(err);
  });
})
