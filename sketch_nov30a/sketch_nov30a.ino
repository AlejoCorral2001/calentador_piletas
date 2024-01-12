// Import required libraries

#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHTesp.h"
#include <ArduinoOTA.h>

#define PUMP_PIN 12
void turn_on_system();
void turn_off_system();

void measure(); //measure function that updates measuring variables

//dht11
#define DHTpin 14    //D5 of NodeMCU is GPIO14
DHTesp dht;
void dht_begin();
void dht_measure(int * HUMEDAD, int * CLIMA);
int CLIMA;
int HUMEDAD;

//sb18b20 pin
const int oneWireBus = 4;
float TEMP1;
float TEMP2;
float temp_diff = 0;  //estimate temp heating value per round
bool invert_sensors = false;
void sb_begin();
void temp_measuring(bool state);  //is water running through sensors?
bool temp_measuring_state();
bool measuring = true;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//light sensor
int LUZ;

//current sensor
long POTENCIA;
int ESTADO = 0;

// Replace with your network credentials
const char* ssid = "X-PRO_CNCs";
const char* password = "fibertel98765";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
void route_server();
void server_begin();

AsyncWebSocket ws("/ws");
void updateClients();
void initWebSocket();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();
  route_server();
  server_begin();
  ArduinoOTA.begin();
  // Start server
  //dht_begin();
  sb_begin();
  pinMode(PUMP_PIN,OUTPUT);
  digitalWrite(PUMP_PIN, 1);
  delay(3000);
}

void loop() {
  measure();
  
  ArduinoOTA.handle();
  ws.cleanupClients();
  updateClients();

  system(); //main procedure that controls pump
  

  delay(1000);
}
