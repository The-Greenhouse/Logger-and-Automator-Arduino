#include "SD.h"
#include"SPI.h"

/*
MOSI - pin 11
MISO - pin 12
CLK - pin 13
CS - pin 10 
*/
bool sd_card_in = true;
const int CSpin = 10;
File sensorData;
int WRITE_DATA_AFTER = 300;  // time in secs, prefer 300(= 5 mins)


struct DataQueary {
  String date;
  String time;
  int t1; int h1;       // Sensor In 1
  int t2; int h2;       // Sensor In 2
  int t3; int h3;       // Sensor In Uncontrolled
  int t4; int h4;       // Sensor Out
};
void setup()
{
Serial.begin(9600);
Serial.print("Initializing SD card...");
pinMode(CSpin, OUTPUT);

if (!SD.begin(CSpin)) {
Serial.println("Card failed, or not present");
sd_card_in = false;
return;
}
Serial.println("card initialized.");
}

void saveData(DataQueary data){
  if(SD.exists("data.csv")){ 
    sensorData = SD.open("data.csv", FILE_WRITE);
    
    String dataString;
    dataString += data.date;
    dataString += ",";
    dataString += data.time;
    dataString += ",";
    dataString += data.t1;
    dataString += ",";
    dataString += data.h1;
    dataString += ",";
    dataString += data.t2;
    dataString += ",";
    dataString += data.h2;
    dataString += ",";
    dataString += data.t3;
    dataString += ",";
    dataString += data.h3;
    dataString += ",";
    dataString += data.t4;
    dataString += ",";
    dataString += data.h4;
    dataString += ",";
    
    if (sensorData){
          sensorData.println(dataString);
          Serial.println(dataString);
      sensorData.close(); // close the file
  }
  }
  else{
    Serial.println("Error writing to file !");
}
}


void loop(){

  DataQueary data = {"__date__", "__time__", 21, 43, 44, 26, 83, 63, 72, 93};
  saveData(data);
  delay(WRITE_DATA_AFTER * 1000);

}
//
