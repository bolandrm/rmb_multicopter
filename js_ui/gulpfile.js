var gulp = require('gulp');
var tinylr = require('tiny-lr');
var exec = require('child_process').exec;
var concat = require('gulp-concat');
var babel = require('gulp-babel');

gulp.task('vendor-scripts', function() {
  return gulp.src([
    'node_modules/underscore/underscore-min.js'
  ])
    .pipe(concat('vendor.js'))
    .pipe(gulp.dest('./dist/'));
});

gulp.task('scripts', function() {
  return gulp.src([
    './scripts/**/*.js'
  ])
    .pipe(concat('app.js'))
    .pipe(babel({ presets: ['stage-1', 'es2015'] }))
    .pipe(gulp.dest('./dist/'));
});

gulp.task('dev', function () {
  var lr = tinylr();
  lr.listen(35729);

  gulp.watch(['**.{css,html}', './.dist/*.js'], function (e) {
    console.log("reloading");

    lr.changed({
      body: {
        files: [e.path]
      }
    });
  });

  gulp.watch(['./scripts/**/*.js'], function(e) {
    gulp.start('scripts');
  });

  gulp.start('scripts');
  gulp.start('vendor-scripts');

  gulp.start('launch');
});

gulp.task('launch', function (cb) {
  var launchApp = '`/Applications/Google\\ Chrome.app/Contents/MacOS/Google\\ Chrome --profile-directory=Default --app-id=alihedogaebgomfcknhkbiikjocomeae`';
  exec(launchApp, function (err, stdout, stderr) {
    console.log(stdout);
    console.log(stderr);
    cb(err);
  });
})
