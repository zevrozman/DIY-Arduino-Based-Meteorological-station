#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <WiFi.h>
#include "ThingSpeak.h"
#include <math.h>
#include <stdio.h>

//------------------------------------------------------

unsigned long myChannelNumber = 1690485;
const char * myWriteAPIKey = "Z7ONGRKGWFX9G39L";

const char* ssid = "HUJI-guest"; // your wifi SSID name
const char* password = "" ;// wifi pasword
 
const char* server = "api.thingspeak.com";

WiFiClient client;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
//-----------------------------------------------------


void setup() {
  Serial.begin(9600);
  if (! sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }


  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

   ThingSpeak.begin(client);
 
   WiFi.begin(ssid, password);
  
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
}
void loop() {
float t = sht31.readTemperature();
float h = sht31.readHumidity();

if (! isnan(t))  {
    Serial.print("Temp *C = "); Serial.println(t);
  }
  else  {
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h))  {
  Serial.print("Hum. % = "); Serial.println(h);
  }
  else  {
  Serial.println("Failed to read humidity");
  }
  
  Serial.println();
  delay(1000);

ThingSpeak.setField(1,t);
  ThingSpeak.setField(2,h);

  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  Serial.println("uploaded to Thingspeak server....");

  client.stop();
 
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  // thingspeak needs minimum 15 sec delay between updates

 
  
  delay(20000);
}
