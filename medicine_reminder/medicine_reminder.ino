#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;

int LED1 = 45;
int LED2 = 46;
int LED3 = 47;
int LED4 = 48;
int LED5 = 49;

int ir1 = 30;
int ir2 = 31;
int ir3 = 32;
int ir4 = 33;
int ir5 = 34;

String daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int leds[5] = {LED1, LED2, LED3, LED4, LED5};
int irs[5] = {ir1, ir2, ir3, ir4, ir5};

int dr_no, dd, hh, mm;
//int buffer = {dr_no, dd, hh, mm};
int counter = 0;

LiquidCrystal_I2C lcd(0x27, 16, 4);

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);

  rtc.begin();

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  lcd.init();
  lcd.begin(16, 4); // initialize the lcd
  
  lcd.backlight();
  lcd.clear();

  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello");
  delay(3000);

  lcd.clear();
  lcd.setCursor(3,1);
  lcd.print("WELCOME TO");
  lcd.setCursor(1,2);
  lcd.print("MEDBOT");
  lcd.setCursor(-2,3);
  lcd.print("by Team Bots");
  delay(3000);
}

void loop() {

  timedisplay();
  bluetoothSetup();
//  Serial1.listen();
  if (Serial1.available()){
    getAlarm();
  }
}

void bluetoothSetup(){

  Serial1.write("AT+ADDR");
//  Serial1.write();

  
}

void isMedicineTaken(int drawer_no){

  int timeNow = millis();

  if (digitalRead(irs[drawer_no-1]) == 0){
    // obstacle is detected
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TAKE YOUR");
    lcd.setCursor(0,1);
    lcd.print("MEDICINE");
    delay(1000);

    if (timeNow + 60000 > millis()){
      sendNotification();
    }
  }
  else{
    // no obstacle, medicine has been taken out
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MEDICINE TAKEN");
    delay(1000);
  }
  
}

void timedisplay(){
  DateTime rtcnow = rtc.now();

//  Serial.println("Current Date & Time: ");
//  Serial.print(rtcnow.year(), DEC);
//  Serial.print('/');
//  Serial.print(rtcnow.month(), DEC);
//  Serial.print('/');
//  Serial.print(rtcnow.day(), DEC);
//  Serial.print(" (");
//  Serial.print(daysOfTheWeek[rtcnow.dayOfTheWeek()]);
//  Serial.print(") ");
//  Serial.print(rtcnow.hour(), DEC);
//  Serial.print(':');
//  Serial.print(rtcnow.minute(), DEC);
//  Serial.print(':');
//  Serial.print(rtcnow.second(), DEC);
//  Serial.println();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Today is :");
  lcd.setCursor(0,1);
  lcd.print(rtcnow.year());
  lcd.setCursor(4,1);
  lcd.print(".");
  lcd.setCursor(5,1);
  lcd.print(rtcnow.month());
  lcd.setCursor(7,1);
  lcd.print(".");
  lcd.setCursor(8, 1);
  lcd.print(rtcnow.day());
  lcd.setCursor(-4,2);
  lcd.print("Time :");
  lcd.setCursor(-4,3);
  lcd.print(rtcnow.hour());
  lcd.setCursor(-2,3);
  lcd.print(":");
  lcd.setCursor(-1,3);
  lcd.print(rtcnow.minute());
  
  delay(1000);
  
}

int getData(){

  int n;
  if (Serial1.available()){
     n = Serial1.read();
   }
  return n;
}

void getAlarm(){

  dr_no = getData();
  dd = getData();
  hh = getData();
  mm = getData();
}


void reminder(int drawer_no, String dd, int hr, int mins){

  DateTime now = rtc.now();
    
  if (daysOfTheWeek[now.dayOfTheWeek()] == dd && hr == now.hour() && mins == now.minute()){
      digitalWrite(leds[drawer_no - 1], HIGH);
      isMedicineTaken(drawer_no);
  }

}

void sendNotification(){

  Serial.println("Medicine not taken. Please alert patient.");
  Serial1.write("Medicine not taken. Please alert patient.");
  
}

// get data from serial monitor
void serial_data(){
  
  Serial.println("Input drawer number :")
  if (Serial.available()){
    dr_no = Serial.read().toInt();
    Serial.println("Drawer no : " + Serial.read());    
  }
  Serial.println("Input day :")
  if (Serial.available()){
    dd = Serial.read().toInt();
    Serial.println("Day : " + daysOfTheWeek[dd-1]));    
  }
  Serial.println("Input time :");
  if (Serial.available()){
    
    Serial.println("time :  " + Serial.read());    
  }
  
  
}
