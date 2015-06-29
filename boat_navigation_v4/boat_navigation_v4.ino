#include <Servo.h>

Servo servo;

const int buttonPin = 2;
const int motorPin = 3;
const int servoPin = 9;
const int sideIRpin = 0;
const int frontIRpin = 3;

int buttonState = LOW;      // the current state of the output pin
int turnState = LOW;      // the current state of turning
int correctionState = 0;      // the current state of adjusting, 1 = left, 2 = right, 0 = straight
int previous = LOW;    // the previous reading from the input pin
int current;           // the current reading from the input pin
int motorspeed = 100;
const int turnAngle = 55;
const int defaultAngle = 90;
const int correctionAngle = 10;
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

int sensVal;           // for raw sensor values 
float filterVal = 0.50;       // this determines smoothness  - .0001 is max  1 is off (no smoothing)
float sideSmoothVal;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing
float frontSmoothVal;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);
  servo.attach(servoPin);

  Serial.begin(9600);                             // start the serial port
}

void loop() {
  if (buttonState == LOW) {
    buttonState = HIGH;
    analogWrite(motorPin,120);  //analogWrite for PWM signals
  }
  
  navigate();
}

void navigate() {
  sideSmoothVal = smooth(analogRead(sideIRpin), filterVal, sideSmoothVal);
  frontSmoothVal = smooth(analogRead(sideIRpin), filterVal, frontSmoothVal);
  
  if (frontSmoothVal > 210) {
    turnState = HIGH;
    servo.write(turnAngle);
  }
  
  while(turnState == HIGH) {
    sideSmoothVal = smooth(analogRead(sideIRpin), filterVal, sideSmoothVal);
    frontSmoothVal = smooth(analogRead(sideIRpin), filterVal, frontSmoothVal);
        
    if (frontSmoothVal < 160) {
      turnState = LOW;
      delay(500);
      servo.write(defaultAngle);
    } else if (frontSmoothVal > 350) {
      servo.write(turnAngle-10);
    }
  }
  
  if (correctionState == 0) {
    if (sideSmoothVal > 280) {
      servo.write(defaultAngle - correctionAngle);
      delay(200);
      servo.write(defaultAngle);
      correctionState = 1;
    } else if (sideSmoothVal < 220) {
      servo.write(defaultAngle + correctionAngle);
      delay(200);
      servo.write(defaultAngle);
      correctionState = 2;
    }
  } else if (correctionState == 1) {
    if (sideSmoothVal < 280) {
      servo.write(defaultAngle - correctionAngle);
      delay(200);
      servo.write(defaultAngle);
      correctionState = 0;
    }
  } else {
    if (sideSmoothVal > 220) {
      servo.write(defaultAngle + correctionAngle);
      delay(200);
      servo.write(defaultAngle);
      correctionState = 0;
    }    
  }
}

int smooth(int data, float filterVal, float smoothedVal){
  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}
