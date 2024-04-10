const express = require('express');
const app = express();

const http = require('http');
const server = http.createServer(app);

const { Server } = require("socket.io");
const io = new Server(server);

const dgram = require('dgram');

const startRouter = require('./lib/page.js');
const {SERVER_PORT: port = 8088} = process.env;
const child = require('child_process');

// Serve static files from the public directory
const path = require('path');
app.use(express.static(path.join(__dirname, 'public')));
app.use('/', startRouter);

io.on('connection', (socket) => {
	console.log('User connected.');

	let ffmpeg = child.spawn("ffmpeg", [
		"-re",
		"-y",
		"-i",
		"udp://192.168.7.1:1234",
		"-preset",
		"ultrafast",
		"-f",
		"mjpeg",
		"pipe:1"
	]);

	ffmpeg.on('error', function (err) {
		console.log(err);
		throw err;
	});

	ffmpeg.on('close', function (code) {
		console.log('ffmpeg exited with code ' + code);
	});

	ffmpeg.stderr.on('data', function(data) {
		// Don't remove this
		// Child Process hangs when stderr exceed certain memory
	});

	ffmpeg.stdout.on('data', function (data) {
		var frame = Buffer.from(data).toString('base64'); //convert raw data to string
		io.sockets.emit('canvas',frame); //send data to client
	});

	handleCommand(socket);
});

server.listen({ port }, () => {
	console.log(`Server is listening on port: ${port}`);
});

function handleCommand(socket) {
	// Pased string of comamnd to relay
	socket.on('daUdpCommand', function(data) {
		console.log('daUdpCommand command: ' + data);

		// Info for connecting to the local process via UDP
		var PORT = 8088;
		var HOST = '192.168.7.2';
		var client = dgram.createSocket('udp4');
		var buffer = new Buffer(data);

		client.bind(54321, '192.168.7.1', () => {
			//console.log('UDP Socket is listening on port 54321');
		});

		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});

		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
			console.log("UDP Client: message Rx " + remote.address + ':' + remote.port +' - ' + message);

			var reply = message.toString('utf8')
			socket.emit('commandReply', reply);

			client.close();
		});

		client.on("UDP Client: close", function() {
			console.log("closed");
		});

		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
	});

};