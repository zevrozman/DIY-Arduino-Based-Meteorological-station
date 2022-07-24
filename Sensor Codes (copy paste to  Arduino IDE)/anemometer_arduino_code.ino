//wind sped anonumeter SM5386:
const int RecordTime = 1; //Define Measuring Time (Seconds)
const int SensorPin = 32;  //Define Interrupt Pin (2 or 3 @ Arduino Uno)

int InterruptCounter;
float WindSpeed;

void setup()
{
  Serial.begin(115200);
  //------------------------------------------------------------------
 
  //------------------------------------------------------------------
  //wind sped anonumeter SM5386:
  pinMode(SensorPin, INPUT);
}

void loop() {
  meassure();
  Serial.print("Wind Speed: ");
  Serial.print(WindSpeed);       //Speed in km/h
  Serial.print(" km/h - ");
  Serial.print(WindSpeed / 0.004); //Speed in m/s
  Serial.println(" m/s");
  
 
  delay(1000);
}

void meassure() {
  InterruptCounter = 0;
  attachInterrupt(digitalPinToInterrupt(SensorPin), countup, RISING);
  delay(1000 * RecordTime);
  detachInterrupt(digitalPinToInterrupt(SensorPin));
  WindSpeed = (float)InterruptCounter / (float)RecordTime * 0.004;
}

void countup() {
  InterruptCounter++;
