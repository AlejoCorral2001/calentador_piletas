#define UMBRAL_PARADA 4
#define UMBRAL_INICIO 12
#define TIEMPO_MEDICION 25
#define PROPORTIONAL_CONSTANT 8
#define LITERS_SECOND 0.45
#define POOL_LITERS 12000
#define MIN_WAIT_TIME 30
#define MAX_WAIT_TIME 480 //8min
//#define HUMBRAL_LUZ 10000

int time_count = 0;
int actualWaitTime = 180;
bool waitTimeRecalculation = false;
bool system_on = false;
int round_time = 0;
int measures_count = 0;
float measures_acc = 0;
int remaining_time = 0;

void pump_turn_on(){
    Serial.println("se encendio la bomba");
    digitalWrite(PUMP_PIN, 1);
    ESTADO = 1;
}

void pump_turn_off(){
    Serial.println("se apago la bomba");
    digitalWrite(PUMP_PIN, 0);
    ESTADO = 0;
}

int pump_state(){
    return(ESTADO);
}

bool check_light(){
    //return LUZ>=HUMBRAL_LUZ;
    return true;    //light testing is not implemented
}

void turn_on_system(){
    system_on = true;
    Serial.println("sistema encendido");
    pump_turn_on();
    temp_measuring(true); //caso unico de inicio
    actualWaitTime = 180;
    time_count = 0;
}

void turn_off_system(){
    system_on = false;
    Serial.println("sistema apagado");
    pump_turn_off();
}

void system(){  //this routine should be run every second
    if(system_on == true && check_light()){
        time_count++;   //we use this variable to get a time reference 
        if(waitTimeRecalculation == true){
            if(time_count >= TIEMPO_MEDICION){  //we must wait to the water to flow and set the correct temperature measurement 
                if(temp_measuring_state() == true){
                  waitTimeRecalculation = false;  //we have will recalculate the new wait Time and this have to be set once
                  if((UMBRAL_INICIO - (TEMP1 - TEMP2)) * PROPORTIONAL_CONSTANT > 100){
                    actualWaitTime = actualWaitTime + 100;
                  }else if((UMBRAL_INICIO - (TEMP1 - TEMP2)) * PROPORTIONAL_CONSTANT < -100){
                    actualWaitTime = actualWaitTime - 100;
                  }else{
                    actualWaitTime = actualWaitTime + (UMBRAL_INICIO - (TEMP1 - TEMP2)) * PROPORTIONAL_CONSTANT;
                  }
                  if(actualWaitTime < MIN_WAIT_TIME){
                    actualWaitTime = MIN_WAIT_TIME;
                  }else if (actualWaitTime > MAX_WAIT_TIME){
                    actualWaitTime = MAX_WAIT_TIME;
                  }
                  Serial.print("tiempo recualculado: ");
                  Serial.println(actualWaitTime);
                  return;                  
                }else{
                  temp_measuring(true);
                  return;                  
                }
            }
        }
        else{
            if(pump_state() == 0){    //encendido de bomba, debemos retocar actualWaitTime para lograr la temp de bajada deseada
                if(time_count >= actualWaitTime){
                  time_count = 0;
                  waitTimeRecalculation = true; //for testing we will not chage this
                  pump_turn_on();
                  Serial.println("se va a recalcular el tiempo");
                  return;
                }else{
                  remaining_time = actualWaitTime - time_count;
                }
            }else if(time_count >= TIEMPO_MEDICION && pump_state() == 1 && ((TEMP1 - TEMP2) <= UMBRAL_PARADA)){ //apagado de bomba, ya no vale la pena seguir moviendo agua
                time_count = 0;
                pump_turn_off();
                temp_measuring(false);
                //NEW TEMP = (LITERS_REMAINING IN POOL * POOL TEMPERATURE + LITERS MOVED TROUGHT SYSTEM  * AVERAGE THRESHOLD) / TOTAL LITERS
                int liters_moved = (measures_count * LITERS_SECOND);
                int liters_remaining = POOL_LITERS - liters_moved;
                float average_threshold =(measures_acc / measures_count);
                float new_temp = (liters_remaining * TEMP2 + liters_moved * (TEMP2 + average_threshold))/POOL_LITERS;
                round_time = measures_count + actualWaitTime;
                temp_diff = new_temp - TEMP2;
                measures_count = 0;
                measures_acc = 0;
                update_google_sheet();
                return;
            }else{
                if(time_count >= TIEMPO_MEDICION && pump_state() == 1){
                  //aca se esta bombeando esperando que baje el umbral
                  //Serial.print("Umbral :");
                  //Serial.println((TEMP1 - TEMP2));
                  measures_count++;
                  measures_acc +=  (TEMP1 - TEMP2);                 
                }
                return;
            }
        }
    }
    else{
        pump_turn_off();
        temp_measuring(false);
        time_count = 0;
    }
    return;
}
