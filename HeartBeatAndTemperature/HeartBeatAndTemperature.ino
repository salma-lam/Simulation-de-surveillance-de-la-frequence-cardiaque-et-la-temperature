#include <TimerOne.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

int val;
int tempPin = A0;// temperature Sensor Pin
int HBSensor = 7;// Sensor Pin
int HBCount = 0;
int HBCheck = 0;
int TimeinSec = 0;
int HBperMin = 0;
int HBStart = 2;
int HBStartCheck = 0;

void setup() 
{
  // put your setup code here, to run once:
  lcd.begin(20, 4);
  pinMode(HBSensor, INPUT);
  pinMode(HBStart, INPUT_PULLUP);
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HEALTH MONITORING");
  lcd.setCursor(0,1);
  lcd.print("TIME IN SEC : ");
  lcd.setCursor(0,3);
  lcd.print("BODY TEMP   : ");
  lcd.setCursor(0,2);
  lcd.print("HB PER MIN  : ");
  
}

void loop() 
{
  val = analogRead(tempPin);
  float mv = (val/1023.0)*5000;
  int cel = mv/10;
  lcd.setCursor(14,3);
  lcd.print(cel);
  lcd.print("   ");
  delay(100);
  
  if(digitalRead(HBStart) == LOW)
  {
    HBStartCheck = 1;
  }
  
  if(HBStartCheck == 1)
  {
    if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
      {
        HBCount = HBCount + 1;
        HBCheck = 1;
      }
      
      if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
      {
        HBCheck = 0;   
      }
      
      if(TimeinSec == 10)
      {
          HBperMin = HBCount * 6;;
          HBStartCheck = 0;
          lcd.setCursor(14,2);
          lcd.print(HBperMin);
          lcd.print(" ");
          HBCount = 0;
          TimeinSec = 0;      
      }
  }
}

void timerIsr()
{
  if(HBStartCheck == 1)
  {
      TimeinSec = TimeinSec + 1;
      lcd.setCursor(14,1);
      lcd.print(TimeinSec);
      lcd.print(" ");
  }
}
