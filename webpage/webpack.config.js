const path = require('path');

var HTMLWebpackPlugin = require('html-webpack-plugin');
var CompressionPlugin = require('compression-webpack-plugin');
var HTMLWebpackPluginConfig = new HTMLWebpackPlugin({
	template: __dirname + '/src/index.html',
	filename: 'index.html',
	inject: 'body'
});



module.exports = env => {
	console.log("Node ENV: " + env.NODE_ENV);
	return {
		mode:  env.NODE_ENV,
		entry: __dirname + '/src/index.js',
		module: {
			rules: [
				{
					test: /\.js$/,
					exclude: /node_modules/,
					use: 'babel-loader'
				},
				{
		          test: /\.jpe?g$|\.ico$|\.gif$|\.png$|\.svg$|\.woff$|\.ttf$|\.wav$|\.mp3$/,
		          use: 'file-loader?name=[name].[ext]'  // <-- retain original file name
		      	}
			]
		},
		output: {
			filename: 'index.js',
			path: __dirname + '/build'
		},
		plugins: [
			HTMLWebpackPluginConfig,
			new CompressionPlugin({
				test: /\.js(\?.*)?$/i,
				exclude: /node_modules/,
				filename: '[path]'
			}),
		]
	}
};