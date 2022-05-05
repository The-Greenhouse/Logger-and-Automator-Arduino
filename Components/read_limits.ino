#include <EEPROM.h>

int temperature_addr = 0;
int temperature_upper = 35;
int temperature_lower = 15;
int temperature_limit = 25;
int temperature_cap = 3;

int humidity_addr = 1;
int humidity_upper = 95;
int humidity_lower = 35;
int humidity_limit = 75;
int humidity_cap = 10;


void read_limits(){
  int t = EEPROM.read(temperature_addr);
  int h = EEPROM.read(humidity_addr);

  if ((t>temperature_upper || t<temperature_lower) || (h>humidity_upper || h<humidity_lower)){
    t = temperature_limit;
    h = humidity_limit;

    EEPROM.write(temperature_addr, t);
    EEPROM.write(humidity_addr, h);
  }
  temperature_limit = t;
  humidity_limit    = h;
}

void setup(){
  Serial.begin(9600);

  read_limits();
  Serial.println(temperature_limit);
  Serial.println(humidity_limit);
  
}

void loop(){
  
}


