#include <ThreeWire.h>  
#include <RtcDS1302.h>


ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup () 
{
    Serial.begin(9600);
    
    Rtc.Begin();
}

#define countof(a) (sizeof(a) / sizeof(a[0]))
String get_time(const RtcDateTime& dt)
{
    char datestring[20];
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u:%02u"),
            dt.Hour(),
            dt.Minute() );
    
    return String(datestring);
}

void loop () 
{
    RtcDateTime now = Rtc.GetDateTime();
    String tm = get_time(now);
    
    Serial.println(tm);


    delay(5000); // five seconds
}
