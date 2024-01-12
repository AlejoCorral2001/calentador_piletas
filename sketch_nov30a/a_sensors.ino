#define TEMP_OFFSET 0.9
void dht_begin(){
    dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
}
void dht_measure(int * HUMEDAD, int * CLIMA){
    *HUMEDAD = dht.getHumidity();
    *CLIMA = dht.getTemperature();
}

//sb18b20


void temp_measuring(bool state){
    measuring = state;
}
bool temp_measuring_state(){
  return measuring;
}

void sb_begin(){
  sensors.begin();  
  Serial.print(sensors.getDeviceCount());
  Serial.println("zondas de temperatura: ");
}

int lastMeasure1;
int lastMeasure2;

void sb_measure(float * TEMP1, float * TEMP2){
  if(measuring == true){
    sensors.requestTemperatures();
    delay(20);
    if(invert_sensors == 0){
        *TEMP1 = sensors.getTempCByIndex(0) + TEMP_OFFSET;
        *TEMP2 = sensors.getTempCByIndex(1);
    }else{
        *TEMP2 = sensors.getTempCByIndex(0) + TEMP_OFFSET;
        *TEMP1 = sensors.getTempCByIndex(1);
    }
  }
}

void light_measure(int * LUZ){
  *LUZ = 0;
}

void measure(){
  //dht_measure(&HUMEDAD, &CLIMA);
  sb_measure(&TEMP1, &TEMP2);
  //light_measure(&LUZ);
  //POTENCIA = (analogRead(A0)*7586);  //0.0029 mV/escalon y cada 100mV son 1 ampere
}
