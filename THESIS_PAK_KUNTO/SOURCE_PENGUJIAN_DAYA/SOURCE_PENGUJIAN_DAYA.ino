#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "UNDEFINED NETWORK";
const char *password = "1qaz2wsx3edc";

const char *host = "89.46.65.90";

void setup() {
  Serial.begin(9600);
  
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

  Serial.print("Arus: ");
  Serial.print(arus);
  Serial.print("\t");
  Serial.print("Daya: ");
  Serial.print(voltage);
  Serial.print(" watt");
  Serial.println();

  PostData(String(voltage), "1500");
  
  delay(500);
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

