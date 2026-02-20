var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

window.addEventListener('load', initWebSocket);

function initWebSocket() {
    websocket = new WebSocket(gateway);
    websocket.onmessage = function(event) {
        document.getElementById('temp-val').innerHTML = event.data;
    };
    websocket.onclose = function() {
        setTimeout(initWebSocket, 2000); // Reconnect
    };
}