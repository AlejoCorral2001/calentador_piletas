const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
    margin-bottom: 4rem;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>CLIMATIZADOR SOLAR DE PILETAS</h1>
  </div>
  <div class="content">
    <div class="card">
      <h3>Zondas Temperatura</h3>
      <h4>Zonda Bajada</h4>
      <p class="state"><span id="TEMP1">%TEMP1%</span></p>
      <h4>Zonda Subida</h4>
      <p class="state"><span id="TEMP2">%TEMP2%</span></p>
      <h4>Diferencia</h4>
      <p class="state"><span id="DIFERENCIA">%DIFERENCIA%</span></p>
      <h4>Midiendo:</h4>
      <p class="state"><span id="MIDIENDO">%MIDIENDO%</span></p>
    </div>
    <div class="card">
      <h3>Datos Del Calentador</h3>
      <h4>Tiempo de calentamiento:</h4>
      <p class="state"><span id="TIEMPO_ESPERA">%TIEMPO_ESPERA%</span></p>
      <h4>Tiempo de ronda:</h4>
      <p class="state"><span id="TIEMPO_RONDA">%TIEMPO_RONDA%</span></p>
      <h4>Tiempo restante:</h4>
      <p class="state"><span id="TIEMPO_RESTANTE">%TIEMPO_RESTANTE%</span></p>
      <h4>Subida estimada por ronda:</h4>
      <p class="state"><span id="TEMPERATURA_SUBIDA">%TEMPERATURA_SUBIDA%</span></p>
    </div>
    <div class="card">
      <h3>Clima</h3>
      <h4>Temperatura</h4>
      <p class="state"><span id="CLIMA">%CLIMA%</span></p>
      <h4>Humedad</h4>
      <p class="state"><span id="HUMEDAD">%HUMEDAD%</span></p>
      <h4>Luz Solar</h4>
      <p class="state"><span id="LUZ">%LUZ%</span></p>
    </div>
    <div class="card">
      <h3>Bomba</h3>
      <h4>Potencia</h4>
      <p class="state"><span id="POTENCIA">%POTENCIA%</span></p>
      <h4>Estado Bomba</h4>
      <p class="state"><span id="ESTADO">%ESTADO%</span></p>
      <h4>Estado Del Sistema</h4>
      <p class="state"><span id="ESTADO_SISTEMA">%ESTADO_SISTEMA%</span></p>
      <p><button id="buttonOn" class="button">Encender sistema</button></p>
      <p><button id="buttonOff" class="button">Apagar sistema</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var chunks = event.data.split(',');
    console.log(chunks);
    for(let i=0; i<chunks.length; i++){
      var Id = chunks[i].split(' ')[0]; 
      var Info = chunks[i].split(' ')[1];      
      console.log(Id);
      console.log(Info);
      document.getElementById(Id).innerHTML = Info;
      }
  }
  
  function onLoad(event) {
    initWebSocket();
    initButtonOn();
    initButtonOff();
  }
  function initButtonOn() {
    document.getElementById('buttonOn').addEventListener('click', toggleOn);
  }
  function initButtonOff() {
    document.getElementById('buttonOff').addEventListener('click', toggleOff);
  }
  function toggleOn(){
    websocket.send('turn on');
  }
  function toggleOff(){
    websocket.send('turn off');
  }
</script>
</body>
</html>
)rawliteral";
  // Route for root / web page

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (1){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void route_server(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
}

void server_begin(){
  server.begin();
}
