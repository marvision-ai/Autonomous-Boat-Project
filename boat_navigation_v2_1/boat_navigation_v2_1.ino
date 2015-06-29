#include <Servo.h>

Servo servo;

const int buttonPin = 2;
const int motorPin = 3;
const int servoPin = 9;
const int sideIRpin = 0;
const int frontIRpin = 3;

int buttonState = LOW;      // the current state of the output pin
int turnState = LOW;      // the current state of turning
int previous = LOW;    // the previous reading from the input pin
int current;           // the current reading from the input pin
int motorspeed = 100;
const int turnAngle = 55;
const int defaultAngle = 90;
const int correctionAngle = 15;
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

const int numReadings = 6;

int sideReadings[numReadings];
int sideIndex = 0;
int sideTotal = 0;
int sideAverage = 0;
int prevSideAverage = 0;
int frontReadings[numReadings];
int frontIndex = 0;
int frontTotal = 0;
int frontAverage = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);
  servo.attach(servoPin);

  // initialize readings in the running average distance to 0
  for (int reading = 0; reading < numReadings; reading++) {
    sideReadings[reading] = 0;
    frontReadings[reading] = 0;
  }
  Serial.begin(9600);                             // start the serial port
}

void loop() {  
  if (buttonState == LOW) {
    buttonState = HIGH;
    analogWrite(motorPin,200);  //analogWrite for PWM signals
  }
  
  navigate();  
}

void navigate() {
  addSideDistance();
  addFrontDistance();
  
  if (frontAverage > 140) {
    turnState = HIGH;
    servo.write(turnAngle);
  }
  
  while(turnState == HIGH) {
    addSideDistance();
    addFrontDistance();
    if (sideAverage > 600) {
      servo.write(turnAngle);
    } else if (sideAverage < 80) {
      servo.write(turnAngle);
    } else {
      servo.write(turnAngle);
    }
    
    if (frontAverage < 125) {
      turnState = LOW;
      delay(500);
      servo.write(defaultAngle);
    }
    if (frontAverage > 350) {
      servo.write(turnAngle-10);
    }
  }
  
  if (sideAverage > 280) {
    servo.write(defaultAngle - correctionAngle);
  } else if (sideAverage < 220) {
    servo.write(defaultAngle + correctionAngle);
  } else {
    straighten();
  }
}

void addSideDistance() {
  sideTotal = sideTotal - sideReadings[sideIndex];
  sideReadings[sideIndex] = analogRead(sideIRpin);
  sideTotal = sideTotal + sideReadings[sideIndex];
  sideIndex = sideIndex + 1;

  if (sideIndex >= numReadings) {
    sideIndex = 0;
  }

  prevSideAverage = sideAverage;
  sideAverage = sideTotal / numReadings;
}

void addFrontDistance() {
  frontTotal = frontTotal - frontReadings[frontIndex];
  frontReadings[frontIndex] = analogRead(frontIRpin);
  frontTotal = frontTotal + frontReadings[frontIndex];
  frontIndex = frontIndex + 1;

  if (frontIndex >= numReadings) {
    frontIndex = 0;
  }

  frontAverage = frontTotal / numReadings;
}

void straighten() {
  if (sideAverage > prevSideAverage + 5) {
    servo.write(80);
  } else if (prevSideAverage < sideAverage - 5) {
    servo.write(100);
  }
}
