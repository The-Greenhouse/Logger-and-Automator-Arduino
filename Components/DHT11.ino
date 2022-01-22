#include "DHT.h"

#define DHTPIN1 7
#define DHTPIN2 2

#define DHTTYPE DHT11 

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 test!");
  dht1.begin();
  dht2.begin();

}

struct DhtData {
  int temperature;
  int humidity;
};

String get_dht_data_string(int temperature, int humidity){
  String data_string = "";

  if(temperature<10)
    data_string+=0;  
  data_string+= temperature;
  data_string+= "C ";

  if(humidity<10)
    data_string+=0;  
  data_string+= humidity;
  data_string+= "%";
    
  return String(data_string);
}



void loop() {
  // DHT
  // DhtData dht1_data = get_dht_data(dht1);
  DhtData dht1_data = {dht1.readTemperature(), dht1.readHumidity()};
  DhtData dht2_data = {dht2.readTemperature(), dht2.readHumidity()};

  Serial.println(get_dht_data_string(
    dht1_data.temperature,  dht1_data.humidity
  ));

  Serial.println(get_dht_data_string(
    dht2_data.temperature,  dht2_data.humidity
  ));

  Serial.println(get_dht_data_string(
    (dht1_data.temperature + dht2_data.temperature) / 2,  
    (dht1_data.humidity + dht2_data.humidity) / 2
    ));

  delay(3000); // wait two seconds
  Serial.println("-------------");

  
  
}




