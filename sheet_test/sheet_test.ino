
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <WiFiClientSecure.h>


// Replace with your network credentials
const char* ssid = "X-PRO_CNCs";
const char* password = "fibertel98765";





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

 
  ArduinoOTA.begin();

  //pinMode(PUMP_PIN,OUTPUT);
  //digitalWrite(PUMP_PIN, 0);

}

void loop() {

  
  ArduinoOTA.handle();


  //int valor = analogRead(0);
  //Serial.println(valor);
  Humidity = 50;
  update_google_sheet();

  delay(10000);
}
