#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include "SD.h"
#include"SPI.h"

#define Heater_pin 14
#define Humidity_pin 15


struct DataQueary {
  String date;
  String time;
  int t1; int h1;       // Sensor-In 1
  int t2; int h2;       // Sensor-In 2
  int t3; int h3;       // Sensor In Uncontrolled
  int t4; int h4;       // Sensor Out
};

#define DHTPIN1 6
#define DHTPIN2 7
#define DHTPIN3 8
#define DHTPIN4 9
#define DHTTYPE DHT11 
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
int READ_AFTER = 3;          // Sensor Reading Taken After | Time in secs

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

//SDA - A4
//SDL - A5
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup_display(){
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void display_info(String time, String data1, String data2, String data3){
  String line1 = time;
  line1 += " IN-";
  line1 += data1;
  
  String line2 = data2;
  line2 += " ";
  line2 += data3;
  
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

ThreeWire myWire(4,3,2); // dat, clk, rst
RtcDS1302<ThreeWire> Rtc(myWire);

String get_time(const RtcDateTime& dt)
{
  String time ;
    int hr = dt.Hour();
    int mn = dt.Minute();

    if(hr<10)
      time+="0";
    time+=hr;
    time+=":";
    if(mn<10)
      time+="0";
    time+=mn;
    
    return String(time);
}

String get_date(const RtcDateTime& dt)
{
  String date ;
    int day   = dt.Day();
    int month = dt.Month();
    int year  = dt.Year();

    date += day;
    date += "/";
    date += month;
    date += "/";
    date += year;
    date += "/";

    return String(date);
}

// MOSI - pin 11
// MISO - pin 12
// CLK - pin 13
// CS - pin 10 
bool sd_card_in = true;
const int CSpin = 10;
File sensorData;
int WRITE_DATA_AFTER = 10;  // time in secs, prefer 300(= 5 mins)

void setup_sd_card(){
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
    dataString += data.date; dataString += ",";
    dataString += data.time; dataString += ",";
    dataString += data.t1;   dataString += ",";
    dataString += data.h1;   dataString += ",";
    dataString += data.t2;   dataString += ",";
    dataString += data.h2;   dataString += ",";
    dataString += data.t3;   dataString += ",";
    dataString += data.h3;   dataString += ",";
    dataString += data.t4;   dataString += ",";
    dataString += data.h4;   dataString += ",";
    
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

// ********SETUP********
void setup()
{
  Serial.begin(9600);
  Serial.println("Project Green House");
  
  setup_display();
  
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  
  Rtc.Begin();

  setup_sd_card();

  pinMode(Heater_pin, OUTPUT);
  pinMode(Humidity_pin, OUTPUT);
}

long itr = 0;
DataQueary data = {"00/00/0000", "00:00", 0,10, 0,10, 0,10, 0,10};

int temp_thresh_hold = 31;
int hum_thresh_hold = 50;

int temp_in_, hum_in_;
void loop(){
  RtcDateTime now = Rtc.GetDateTime();
  String date_ = get_date(now);
  String time_ = get_time(now);

  temp_in_ = (data.t1 + data.t2) / 2;
  hum_in_  = (data.h1 + data.h2) / 2;

  if (temp_in_ < temp_thresh_hold){
    digitalWrite(Heater_pin, HIGH);
  } else{
   digitalWrite(Heater_pin, LOW); 
  }

  if (hum_in_ < hum_thresh_hold){
    digitalWrite(Humidity_pin, HIGH);
  } else{
   digitalWrite(Humidity_pin, LOW); 
  }

  String env_1_data = get_dht_data_string(                      // Inside the Controlled Greenhouse ENV
    (data.t1 + data.t2) / 2,
    (data.h1 + data.h2) / 2
    );
  String env_2_data = get_dht_data_string(                      // Inside the unControlled Greenhouse ENV
    data.t3,  data.h3
    );
  String env_3_data = get_dht_data_string(                      // Outside the  Greenhouse 
    data.t4,  data.h4
    );

  if(itr%READ_AFTER==0){
    data = {date_, time_, 
            dht1.readTemperature(), dht1.readHumidity(), 
            dht2.readTemperature(), dht2.readHumidity(), 
            dht3.readTemperature(), dht3.readHumidity(), 
            dht4.readTemperature(), dht4.readHumidity()};
    }
    
  if(itr%WRITE_DATA_AFTER==0){
      saveData(data);
  }

  display_info(time_, env_1_data, env_2_data, env_3_data);
  delay(1000);
  itr+=1;
}
