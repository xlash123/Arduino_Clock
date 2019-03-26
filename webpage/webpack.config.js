var HTMLWebpackPlugin = require('html-webpack-plugin');
var HTMLWebpackPluginConfig = new HTMLWebpackPlugin({
	template: __dirname + '/src/index.html',
	filename: 'index.html',
	inject: 'body'
});

module.exports = {
	mode: 'production',
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
		filename: 'compiled.js',
		path: __dirname + '/build'
	},
	plugins: [HTMLWebpackPluginConfig]
};