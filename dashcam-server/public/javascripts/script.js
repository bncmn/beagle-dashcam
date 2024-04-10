const socket = io();
socket.on("connect", (socket) => { //confirm connection with NodeJS server
    console.log("Connected");
});

$( document ).ready(function() {
    console.log("Document loaded");
    // Setup a repeating function (every 1s)
    // Reference: Using HttpsProcTimer code
	window.setInterval(function() {
        sendCommandViaUDP("time");
    }, 1000);

    $('#toggle').change(function() {
        // Check if the checkbox is checked
        if ($(this).is(':checked')) {
            // Add class to change background color to grey when checked
            sendCommandViaUDP("motionOn");
        } else {
            // Remove class to change background color to blue when unchecked
            sendCommandViaUDP("motionOff");
        }
    });

    $('#stop').click(function(){
        sendCommandViaUDP("stop");  // terminating
    });

    // Listening to reply from UDP server
    socket.on('commandReply', function(result) {
        processReply(result);
    });

    socket.on('canvas', function(data) {
        const canvas = $("#videostream");
        const context = canvas[0].getContext('2d');
        const image = new Image();
        image.src = "data:image/jpeg;base64,"+data;
        image.onload = function(){
            context.height = image.height;
            context.width = image.width;
            context.drawImage(image,0,0,context.width, context.height);
        }
    });
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
};

function processReply(command) {
    const words = command.split(" ");
	var operation = words[0];
	console.log('Message received: '+words[0]);

	switch(operation) {
        
        case "time":
            var deviceTime = Number(words[1]);  // time in seconds
            var deviceHour = Math.floor(deviceTime / 60 / 60);
            var deviceMin = Math.floor((deviceTime - deviceHour * 60 * 60) / 60);
            var deviceSec = Math.floor(deviceTime - (deviceHour * 60 * 60) - (deviceMin * 60));

            $('#status').text("Device up for: "+deviceHour+":"+deviceMin+":"+deviceSec+"(H:M:S)");
            break;
        
        default:
            break;
	}
}