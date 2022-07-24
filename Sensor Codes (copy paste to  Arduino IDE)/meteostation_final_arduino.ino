#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <WiFi.h>
#include "ThingSpeak.h"
#include <math.h>
#include <stdio.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>//barometer
#include <Adafruit_ADS1X15.h> //for radiation Sensor


#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version for radiation sensor*/
//------------------------------------------------------
//
unsigned long myChannelNumber = ;//you channel number here
const char * myWriteAPIKey = "";//your API here
//agrotech-lab-1, 1Afuna2Gezer
const char* ssid = ""; // your wifi SSID name
const char* password = "" ;// wifi pasword
 
const char* server = "api.thingspeak.com";
//windspeed_______________________________________________

const int RecordTime = 1; //Define Measuring Time (Seconds)
const int SensorPin = 32;  //Define Interrupt Pin (2 or 3 @ Arduino Uno)

int InterruptCounter;
float WindSpeed;

//_____________________________________________

WiFiClient client;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// VPD calc__
// the following is for VPD calculation
float e;
float m;
float SVP;
float VPD;
float f;
float whatsTheVpd (float h, float tempeture) {
  e = 2.71828;
  m = tempeture / (tempeture + 238.3) * 17.2694;
  SVP = 610.78 * pow(e, m);
  f = 1 - h / 100;
  VPD = SVP * f;
  return VPD / 1000;

}




//-----------------------------------------------------


void setup() {
  Serial.begin(115200);
  if (! sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
//windspeed_____________________________________________
  pinMode(SensorPin, INPUT);
  
  //barometric__________________________________________
{
    while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  
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
  
//Radiation sensor
 ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
   if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
}
}

void loop() {
  
int16_t results; //radiation sensor 
results = ads.readADC_Differential_0_1();//raditation sensor

float t = sht31.readTemperature();
float h = sht31.readHumidity();
float vpd = whatsTheVpd(h, t);
float rN=((results*0.03125)/0.277)*86400/1000000 ;//radiation conversion from w/m2 to megajoule/m2*day
// ETo calc part 1
float es_part1= 17.27*t;
float es_part2=237.3+t;
float es_part3=(es_part1/es_part2);
float es_final=pow(0.61078,es_part3);

// ETo calc part 2
float delta_part1=4098*es_final;
float delta_part2=pow(es_part2,2);
float delta_final=(delta_part1/delta_part2);


//ETo calc part 3
float Kc= 1;
float et0_part1=2.45*(delta_final+0.067*(1+0.33*WindSpeed));
float et0_final=Kc* (((delta_final*rN)+0.5*vpd*WindSpeed)/et0_part1);
Serial.print("ETo mm/day="); Serial.println(et0_final);


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
  
 //windspeed_________________________
  meassure();
  Serial.print("Wind Speed: ");
  Serial.print(WindSpeed );// 0.004); //Speed in m/s
  Serial.println(" m/s");
  
   
//barometric___________-
{
   Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

    Serial.println();
    delay(2000);
//VPD  
  Serial.print("VPD = "); Serial.println(vpd);
//radiation
  Serial.print("megajoule/m2*day"); Serial.println(rN); 
}

ThingSpeak.setField(1,t);
  ThingSpeak.setField(2,h);
  ThingSpeak.setField(3,bmp.readPressure()); 
  ThingSpeak.setField(4,bmp.readAltitude(1013.25));
  ThingSpeak.setField(5,WindSpeed);
  ThingSpeak.setField(6,vpd);
  ThingSpeak.setField(7,et0_final);
  ThingSpeak.setField(8,rN);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  Serial.println("uploaded to Thingspeak server....");

  client.stop();
 
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  // thingspeak needs minimum 15 sec delay between updates

 
  
  delay(20000);
}

//windspeed thing___________________
void meassure() {
  InterruptCounter = 0;
  attachInterrupt(digitalPinToInterrupt(SensorPin), countup, RISING);
  delay(1000 * RecordTime);
  detachInterrupt(digitalPinToInterrupt(SensorPin));
  WindSpeed =1; // add calibration factor here if known: ((float)InterruptCounter / (float)RecordTime * 0.004)*0.00424;//calibrated 
}

void countup() {
  InterruptCounter++;
}
