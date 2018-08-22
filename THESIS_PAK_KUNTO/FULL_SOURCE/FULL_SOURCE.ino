#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "UNDEFINED NETWORK";
const char *password = "1qaz2wsx3edc";
const char *host = "89.46.65.90";

const int numreadings = 10;
int readings[numreadings];
unsigned long average = 0;
int urutan;
unsigned long total;

volatile int rpmcount = 0;
unsigned long rpm = 0;
unsigned long lastmillis = 0;

void setup() {
  Serial.begin(9600);

  attachInterrupt(0, rpm_fan, FALLING);
  urutan = 0;
  
  delay(1000);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  int sensorValue = analogRead(A0);
  int voltage = map(sensorValue, 1, 1024, 1, 256);
  int arus = 220*voltage;

  if(millis() - lastmillis >= 1000){
    detachInterrupt(0);
    total = 0;  
    readings[urutan] = rpmcount * 60;
    for (int x=0; x<=9; x++){
      total = total + readings[x];
    }
  average = total / numreadings;
  rpm = average;
  rpmcount = 0;
  urutan++;
  if(urutan >= numreadings){
    urutan=0; 
  } 
  if (millis() > 11000){
    Serial.print("Daya: ");
    Serial.print(arus);
    Serial.print("\t");
    Serial.print("Arus: ");
    Serial.print(voltage);
    Serial.print(" watt");
    Serial.print("Kecepatan: ");
    Serial.print(rpm);
    Serial.print(" rpm");
    Serial.println();
  
    PostData(String(voltage), String(rpm));
  }
  lastmillis = millis();
    attachInterrupt(0, rpm_fan, FALLING);
  }
}

void PostData(String power, String rpm){
  HTTPClient http;

  http.begin("http://89.46.65.90/my_thesis/do_sensor.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST("power="+power+"&rpm="+rpm);
  String payload = http.getString();

  Serial.print("Code: ");
  Serial.print(httpCode);
  Serial.print("Response: ");
  Serial.println(payload);
 
  http.end();
}

void rpm_fan(){
  rpmcount++;
}
