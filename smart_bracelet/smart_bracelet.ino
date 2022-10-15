#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

const char *ssid     = "<YOUR WIFI SSID>";
const char *password = "<YOUR WIFI PASSWORD>";

String days_arr[5] = {"","","","",""};
int hours_arr[5] = {0,0,0,0,0};
int mins_arr[5] = {0,0,0,0,0};

int LED = 13;
int buzzer = 4;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

ESP8266WebServer server(80); //Server on port 80

const char MAIN_page[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
          form{
            width: 100%;
            color: darkcyan;
            height: 20%;
            margin-left: 40%;
          }
          h1{
            color: darkgreen;
            text-align: center;
            font-family: Arial, Helvetica, sans-serif;
            font-style: bold;
          }
          h2{
            color : darkblue;
            text-align: center;
            font-family: Arial, Helvetica, sans-serif;
            font-style: italic;
          }
        </style>
        <title>Document</title>
    </head>
    <body>
        <header>
            <h1> MEDBOT </h1>
            <h2> Medicine Intake </h2>
        </header>
        
        <form action="/action_page">
            <label>Drawer number : </label><br>
            <input type="number" name="drawer_no"><br>
            <label>Day :</label><br>
            <input type="text" name="day"> <br>
            <label>Times (hour) : </label><br>
            <input type="number" name="hour"> <br>
            <label>Time (minute) :</label><br>
            <input type="number" name="mins"> <br><br>
            <input type="submit" value="Submit">
        </form>              
        
    </body>
  </html>
)=====";

//SoftwareSerial ard_com(3,1); //RX, TX

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleForm(){
  String drawer_no = server.arg("drawer_no");
  String day = server.arg("day");
  String hour = server.arg("hour");
  String mins = server.arg("mins");

  Serial.println("drawer no : " +  drawer_no);
  Serial.println("On this day: " +  day);
  Serial.println(" at : " +  hour + ":" + mins);

  days_arr[drawer_no.toInt() - 1] = day;
  hours_arr[drawer_no.toInt() - 1] = hour.toInt();
  mins_arr[drawer_no.toInt() - 1] = mins.toInt();

  String s = "<a href='/'> Go Back </a>";
  server.send(200, "text/html", s); //Send web page
}


void setup() {

  Serial.begin(115200);
//  ard_com.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.begin(ssid, password);     //Connect to your WiFi router

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected successfully!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); 

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(19800);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();          //Handle client requests
  timeClient.update();

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime); 
  Serial.println("Medicine intake drawer 1 at " + days_arr[0] + " , " + hours_arr[0] + ":" + mins_arr[0]);
  Serial.println("Medicine intake drawer 2 at " + days_arr[1] + " , " + hours_arr[1] + ":" + mins_arr[1]);
  Serial.println("Medicine intake drawer 3 at " + days_arr[2] + " , " + hours_arr[2] + ":" + mins_arr[2]);
  Serial.println("Medicine intake drawer 4 at " + days_arr[3] + " , " + hours_arr[3] + ":" + mins_arr[3]);
  Serial.println("Medicine intake drawer 5 at " + days_arr[4] + " , " + hours_arr[4] + ":" + mins_arr[4]);

  Serial.println("");

  for (int i=0; i < 5; i++){
    if (weekDays[timeClient.getDay()] == days_arr[i] && timeClient.getHours() == hours_arr[i] && timeClient.getMinutes() == mins_arr[i]){
      Serial.println("MEdicine intake");
      digitalWrite(LED, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      delay(30000);
    }
  }
  digitalWrite(LED, LOW);
  digitalWrite(buzzer, LOW);
  delay(2000);
  Serial.println("");
//  sendToArduino();
}

//void sendToArduino(){
//
//  for (int i=0; i<5; i++){
//    ard_com.write(days_arr[i]);
//    ard_com.write(hours_arr[i]);
//    ard_com.write(mins_arr[i]);
//  }
//}
