//sketch created by Akshay Joseph
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Fixed Depending upon the board
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
  line2 += " -";
  line2 += data3;
  
  
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
  
  setup_display();
  
  lcd.setCursor(0,0);
  lcd.print("NOTHING HAPPNESD");
  lcd.setCursor(0,1);
  lcd.print("NOTHING HAPPNESD");
}

int i = 0;
void loop()
{
  delay(1000);
  String s;
  s+=i++; s+=i++; s+=i++; s+=i++; s+=i++;
  display_info("12:34", "56C 78%", "90C 12%", "34C 56%");
//  delay(1000);
}
