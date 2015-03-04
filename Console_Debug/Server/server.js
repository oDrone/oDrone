#!/usr/bin/env node

var WebSocketServer = require('websocket').server;
var http = require('http');

var server = http.createServer(function(request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});

var drone = null
var command = null

server.listen(9000, function() {
    console.log((new Date()) + ' Server is listening on port 9000');
});

wsServer = new WebSocketServer({
    httpServer: server,
    autoAcceptConnections: false
});

wsServer.on('request', function(request) {

    var connection = request.accept('odrone-protocol', request.origin);

    console.log((new Date()) + ' Connection accepted.');
    //console.log(request.httpRequest);
    connection.url = request.httpRequest.url;

    connection.on('message', function(message) {

        if (message.type !== 'utf8') {
            connection.close();
            return;
        }

        console.log("Url: " + connection.url);
        console.log('Received Message: ' + message.utf8Data);

        if (connection.url == "/command") {

            // Check login
            if (command == null) {

                if (message.utf8Data !== "auth") {
                    connection.close();
                    return;
                }

                command = connection;
            } else if (drone != null) {
                drone.sendUTF(message.utf8Data);
            } else {
                connection.sendUTF("No drone");
            }
        } else if (connection.url == "/drone") {

            // Check login
            if (drone == null) {

                if (message.utf8Data !== "auth") {
                    connection.close();
                    return;
                }

                drone = connection;
            } else if (command != null) {
                command.sendUTF(message.utf8Data);
            } else {
                connection.sendUTF("No command");
            }
        }
    });
    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');

        if (connection == drone) {
            drone = null
        } else if (connection == command) {
            command = null
        }
    });
});
