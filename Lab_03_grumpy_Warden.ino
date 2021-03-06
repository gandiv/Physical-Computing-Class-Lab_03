/*
Lab 3 - GrumpyWarden
Parth Soni
CDIGF 2B03 Physical Computing S01
OCAD University
Created on [Jan 25th 2015]
 
Based on:
Tutorials in the readings
*/

/*
So I've generalized the concept, and made a little demo to present how the system would work.
This program basiclaly will calibrate for 10 seconds when it starts. The user will have to
introduce the dark room values and bright room values in those 10 seconds. This could be done 
at night time just by fliping the light on and off. Same goes for the IR sensor calibration. The user
can just put something in front and remove it. This will calibrate the program.

Then once that is done, the values will be contrained and smoothed for best result. If
any funny readings are picked up then it will, serial print a warden message!
*/


#include <Servo.h> 

//General
const int numReadings = 10;
int index = 0; //Index of the current reading

int LDRreadings[numReadings];
int LDRtotal = 0; // The total of all of the index readings
int LDRaverage = 0; //average of the total index readings

int IRreadings[numReadings];
int IRtotal = 0; // The total of all of the index readings
int IRaverage = 0; //average of the total index readings

//LED
int LEDPin = 8;

//Just the Servo
Servo servo_1;  // create servo object to control a servo 
int pos = 0;    // variable to store the servo position 
int servoPin = 9;

//Just for the LDR
int LDRValue = 0;
int LDRSensorMin = 1023; // minimum sensor value
int LDRSensorMax = 0; // maximum sensor value
int LDRPin = A1;

//Just for the IR
int IRValue = 0;
int IRSensorMin = 1023; // minimum sensor value
int IRSensorMax = 0;  // maximum sensor value
int IRPin = A0;

int LDRApprox = 130; // These are to rough out the values further
int IRApprox = 130; // These are to rouch out the values further

void setup() {

  pinMode (servoPin, OUTPUT);
  pinMode (LEDPin, OUTPUT);

  Serial.begin(9600);

  digitalWrite (LEDPin, HIGH);
  servo_1.attach(servoPin);

  Serial.println("Your Calibration will Start now...");
  startCalibration(); // This will start the calibration
  Serial.println("Your Calibrationis Complete...");
  Serial.println(IRSensorMax);

  digitalWrite (LEDPin, LOW);

}

void loop () {
  
  for(pos = 0; pos < 180; pos += 1)  
  {                              
    servo_1.write(pos);           
    averageFunc();
    delay(15);                      
  } 
  for(pos = 180; pos>=1; pos-=1) 
  {                                
    servo_1.write(pos);  
   averageFunc();
    delay(15);   
  }
 
  }

void startCalibration () {

  while (millis() < 10000) {

    // JUST FOR THE LDR
    LDRValue = analogRead(LDRPin);
    // record the maximum Light sensor value
    if (LDRValue > LDRSensorMax) {
      LDRSensorMax = LDRValue;
    }
    // record the minimum Light sensor value
    if (LDRValue < LDRSensorMin) {
      LDRSensorMin = LDRValue;
    }

    //JUST FOR THE IR
    IRValue = analogRead(IRPin);
    // record the maximum IR sensor value
    if (IRValue > IRSensorMax) {
      IRSensorMax = IRValue;
    }
    // record the minimum IR sensor value
    if (IRValue < IRSensorMin) {
      IRSensorMin = IRValue;
    }

  }
}

void averageFunc () {
  
  //FOR LDR
  // subtract the last reading:
  LDRtotal= LDRtotal - LDRreadings[index];         
   // read from the sensor:  
  LDRreadings[index] = analogRead(LDRPin); 
  LDRreadings[index] = constrain(LDRreadings[index], LDRSensorMin, LDRSensorMax);
  // add the reading to the total:
  LDRtotal= LDRtotal + LDRreadings[index];       
  
    //FOR IR
    // subtract the last reading:
  IRtotal= IRtotal - IRreadings[index];         
   // read from the sensor:  
  IRreadings[index] = analogRead(IRPin); 
  IRreadings[index] = constrain(IRreadings[index], IRSensorMin, IRSensorMax);
  // add the reading to the total:
  IRtotal= IRtotal + IRreadings[index]; 
  
  //INDEX AGAIN
  // advance to the next position in the array:  
  index++;;                    
  // if we're at the end of the array...
  if (index >= numReadings)      {        
    // ...wrap around to the beginning: 
   index = 0;                     }
   
   //BACK TO LDR
  // calculate the average:
  LDRaverage = LDRtotal / numReadings; 

   //BACK TO IR
  // calculate the average:
  IRaverage = IRtotal / numReadings; 
  
  checkForConditions ();
  
  delay(1);        
}

void checkForConditions () {
  
  if (LDRaverage > LDRSensorMin+LDRApprox){
    Serial.println ("Warden! This Room's lights are on!");
    digitalWrite (LEDPin, HIGH);    
  }
  
else if (IRaverage > IRSensorMax-IRApprox){
   Serial.println ("Warden! The Kids have placed something in front of the Light sensor!!");
   digitalWrite (LEDPin, HIGH);    

  }
  else {
     digitalWrite (LEDPin, LOW);    
    
  }

  
}
