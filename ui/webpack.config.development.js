'use strict';

const webpack = require('webpack');
const webpackTargetElectronRenderer = require('webpack-target-electron-renderer');
const baseConfig = require('./webpack.config.base');


const config = Object.create(baseConfig);

config.debug = true;

config.devtool = 'cheap-module-eval-source-map';

config.entry = [
  'webpack-hot-middleware/client?path=http://localhost:4567/__webpack_hmr',
  './app/index'
];

config.output.publicPath = 'http://localhost:4567/dist/';

config.module.loaders.push(
  {
    test: /\.s?css$/,
    loaders: [
      'style-loader',
      'css-loader?sourceMap',
      'sass-loader'
    ]
  },
  { test: /\.eot(\?v=\d+\.\d+\.\d+)?$/, loader: 'file' },
  { test: /\.(woff|woff2)$/, loader:'url?prefix=font/&limit=5000' },
  { test: /\.ttf(\?v=\d+\.\d+\.\d+)?$/, loader: 'url?limit=10000&mimetype=application/octet-stream' },
  { test: /\.svg(\?v=\d+\.\d+\.\d+)?$/, loader: 'url?limit=10000&mimetype=image/svg+xml' }
)


config.plugins.push(
  new webpack.HotModuleReplacementPlugin(),
  new webpack.NoErrorsPlugin(),
  new webpack.DefinePlugin({
    '__DEV__': true,
    'process.env': {
      'NODE_ENV': JSON.stringify('development')
    }
  })
);

config.target = webpackTargetElectronRenderer(config);

module.exports = config;
