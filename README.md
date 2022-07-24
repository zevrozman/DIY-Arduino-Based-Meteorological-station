# DIY-Arduino-Based-Meteorological-Upload to thingspeak 
Using a combination of meteorological sensors to calculate daily evapotranspiration rates according to the Penman Monteith equation
Hi All!
Quick shout out to the course that got me into this world of fun- Agrotechnology Lab at the Hebrew University Faculty of Agriculture- [link]("https://agrotech-lab.github.io/")
Follow the steps provided [link]("https://agrotech-lab.github.io/code/2022/03/07/intro") to install Arduino and set it to the settings for using the ESP32 microcontroller board (used in this project)
You will also need to create an account on thingspeak to be able to upload the data from the sensors to this cloudspace, visualize it in RealTime, and export it for analysis [link]("https://thingspeak.com/").  

To get started with this project you will need:
To download the Arduino Platform 
List of components:
ESP32 microcontroller
GY-b11 280 barometric pressure sensor (also measures temperature and RH, but only pressure (Pa) included in final merged code)
SHT3X temperature and Relative Humidity Sensor (also used to calculate VPD in kPa)
ADS1115 module (increases resolution- 2^16 bits)
Solar Radiation Sensor- model RY EBN1, sensitivity 277.01µV to W/M^2
Wind three cup anemometer (not included in above picture because unable to calibrate. Included in code but set to a constant of 1m/s windspeed)
Voltage Buck converter (to set a voltage input (probably 12V) as a 5V output to the ESP32) 
Breadboard
Male-male and male-female jumper wires
Electrical box
