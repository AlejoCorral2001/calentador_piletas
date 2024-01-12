
void updateClients() {
  //ws.textAll(String(ledState));
  char msg[200];
  sprintf(msg, "TEMP1 %.2f°,TEMP2 %.2f°,DIFERENCIA %.2f,TIEMPO_ESPERA %dm:%ds,TIEMPO_RONDA %dm,TIEMPO_RESTANTE %ds,MIDIENDO %d,TEMPERATURA_SUBIDA %.3f°,CLIMA %d°,HUMEDAD %d%%,POTENCIA %dW,ESTADO %d,LUZ %dl/cm2, ESTADO_SISTEMA &d"
                ,TEMP1, TEMP2,(TEMP1 - TEMP2), ((int) (actualWaitTime/60)),((actualWaitTime-((int) (actualWaitTime/60))*60))  ,((int) round_time/60), remaining_time, measuring, temp_diff, CLIMA, HUMEDAD, POTENCIA, ESTADO, LUZ, system_on);
  ws.textAll(msg);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "turn on") == 0) {
      turn_on_system();
    }else if(strcmp((char*)data, "turn off") == 0){
      turn_off_system();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
