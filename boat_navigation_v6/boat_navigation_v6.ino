#include <Servo.h>

Servo servo;

const int buttonPin = 2;
const int motorPin = 3;
const int servoPin = 9;
const int sideIRpin = 0;
const int frontIRpin = 3;

int buttonState = LOW;      // the current state of the output pin
int turnState = LOW;      // the current state of turning
int adjustState = 0;      // the current state of adjusting, 1 = left, 2 = right, 0 = straight

int curMotorspeed;
const int minMotorspeed = 150;
const int maxMotorspeed = 255;

const int turnAngle = 55;
const int defaultAngle = 90;
const int adjustAngle = 10;
const int correctionAngle = 5;
const int straightenAngle = 5;

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
  curMotorspeed = minMotorspeed;
  
  Serial.begin(9600);                             // start the serial port
}

void loop() {  
  if (buttonState == LOW) {
    buttonState = HIGH;
    analogWrite(motorPin,curMotorspeed);  //analogWrite for PWM signals
  }
  
  navigate();
  //accelerate();
}

void navigate() {
  addSideDistance();
  addFrontDistance();
  
  if (frontAverage > 140) {
    turnState = HIGH;
    servo.write(turnAngle);
  }
  
  if (sideAverage > 300) {
    servo.write(defaultAngle - adjustAngle);
  } else if (sideAverage < 220) {
    servo.write(defaultAngle + adjustAngle);
  } else {
    straighten();
  }
}

void turn() {
  while(turnState == HIGH) {
    addSideDistance();
    addFrontDistance();
    
    if (frontAverage < 125) {
      turnState = LOW;
      delay(500);
      servo.write(defaultAngle);
    }
    if (frontAverage > 600) {
      analogWrite(motorPin, 0);
      while (1) {};
    }
  }  
}

void straighten() {
  if (sideAverage > prevSideAverage + 5) {
    servo.write(defaultAngle - straightenAngle);
  } else if (prevSideAverage < sideAverage - 5) {
    servo.write(defaultAngle + straightenAngle);
  }
}

void accelerate() {
  if (frontAverage < 100 && curMotorspeed != maxMotorspeed) {
    curMotorspeed += 5;
    analogWrite(motorPin, curMotorspeed);
  } else if (curMotorspeed != minMotorspeed) {
    curMotorspeed -= 5;
    analogWrite(motorPin, curMotorspeed);
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
