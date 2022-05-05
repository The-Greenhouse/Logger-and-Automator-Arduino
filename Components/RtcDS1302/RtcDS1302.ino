#include <ThreeWire.h>
#include <RtcDS1302.h>


ThreeWire myWire(4, 3, 2);  // dat, clk, rst
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() 
{
    Serial.begin(9600);

    Rtc.Begin();
}


String get_time(const RtcDateTime& dt)
{
  String time ;
    int hr = dt.Hour();
    int min = dt.Minute();
    
    if(hr<10)
      time+="0";
    time+=hr;
    time+=":";
    if(min<10)
      time+="0";
    time+=min;
  
    return time;
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


//src  - https://electropeak.com/learn/interfacing-ds1302-real-time-clock-rtc-module-with-arduino/

void loop() {
  RtcDateTime now = Rtc.GetDateTime();
  String tm = get_time(now);

  Serial.println(tm);
  Serial.println(get_date(now));


  delay(5000);  // five seconds
}
