// Globals
var sock = null
var connected = false

function connect() {

    if (!connected) {
        sock = new WebSocket("ws://" + document.getElementById("connect_url").value +  "/command", "odrone-protocol");
        sock.onopen = function(event) {
            console.log("OnOpen"); 
            connected = true;
            sock.send("auth");


            s = document.getElementById("status");
            s.style.color = "green";
            s.innerHTML = "connected";

            document.getElementById("connect_button").innerHTML = "Disconnect";
        }
        sock.onclose = function(event) {
            console.log("OnClose");
            console.log(event);
            connected = false;

            s = document.getElementById("status");
            s.style.color = "red";
            s.innerHTML = "disconnected";

            document.getElementById("connect_button").innerHTML = "Connect";
        }
        sock.onmessage = function(event) { onMessage(event.data); }
        sock.onerror = function(event) { console.log("OnError"); console.log(event); }
    } else {
        sock.close();
    }
}

function sendString(str) {
    if (connected) {
        sock.send(str);
        document.getElementById("keys").innerHTML = str + "<br />" + document.getElementById("keys").innerHTML;
    }
}

function onMessage(str) {

    document.getElementById("debug").innerHTML = str + "<br />" + document.getElementById("debug").innerHTML;
}

document.addEventListener('keydown', function(event) {

    sendString("keydown " + event.keyCode); 
});

document.addEventListener('keyup', function(event) {

    sendString("keyup " + event.keyCode); 
});
