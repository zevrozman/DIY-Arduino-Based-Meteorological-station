# DIY-Arduino-Based-Meteorological Station -Upload data to a Thingspeak channel
Hi All!

In this project we will use a combination of meteorological sensors to calculate daily evapotranspiration rates according to the Penman Monteith equation. The sensors will upload the data from all the included sensors to Thingspeak 3 times a minute, and be used together to calculate evapotranspiration. 

What is Evapotranspiration (ET)? Well it is the amount of water that leaves the soil ("evapo") and the pores of leaves ("transpiration"). In a straw like manner, the atmosphere pulls water out of plants and soil, due to having a lower water potential than moist soil or the xylem of plants. Vapor Pressure Defecit (VPD) indicates how much water the atmosphere is lacking from its maximum capacity (at a given tempurature and barometric pressure). Relative Humidity indicates how close the atmosphere is to its 100% capacity. So on a very humid day (a lot of water in the air) the atmosphere has less "room" to pull in more water, since it closer to its maximum capacity, and ET rates will be lower (all other parameters being equal). Transpiration is important for plants because it allows them to do everything they need (bring water and nutrients from the soil and through their tissue, photosynthesis, esc..
ET is important for us to moniter because it can give us a good indication of how much to water our crops, thereby reducing water waste. It is measured in (mm/time). This is because it is cubic meters of water (volume) over meters squared of ground (Area). 

Well I guess thats enough of that. 

Quick shout out to the course that got me into this world of fun- Agrotechnology Lab at the Hebrew University Faculty of Agriculture- [link](https://agrotech-lab.github.io/)

Follow the steps provided [link](https://agrotech-lab.github.io/code/2022/03/07/intro) to install Arduino and set it to the settings for using the ESP32 microcontroller board (used in this project)

You will also need to create an account on thingspeak to be able to upload the data from the sensors to this cloudspace, visualize it in RealTime, and export it for analysis [link](https://thingspeak.com/)  

To get started with this project (after properly installing Arduino) you will need,
List of components:

ESP32 microcontroller

Mini USB cable 

GY-b11 280 barometric pressure sensor (also measures temperature and RH, but only pressure (Pa) included in final merged code)

SHT3X temperature and Relative Humidity Sensor (also used to calculate VPD in kPa)

ADS1115 module (increases resolution- 2^16 bits)

Solar Radiation Sensor- model RY EBN1, sensitivity 277.01µV to W/M^2

Wind three cup anemometer (not included in above picture because unable to calibrate. Included in code but set to a constant of 1m/s windspeed)

Voltage Buck converter (to set a voltage input (probably 12V) as a 5V output to the ESP32) 
Breadboard

Male-male and male-female jumper wires

Electrical box

Let's get started!
