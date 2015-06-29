#include <Servo.h>

Servo servo;

const int motorPin = 3;
const int servoPin = 9;

int buttonState =LOW;      // the current state of the output pin
int motorspeed = 100;

void setup() {
  pinMode(motorPin, OUTPUT);
  servo.attach(servoPin);
  Serial.begin(9600);                             // start the serial port
}

void loop() {
  if (buttonState == LOW) {
    buttonState = HIGH;
    analogWrite(motorPin,motorspeed);  //analogWrite for PWM signals
  }
  
  servo.write(110);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(70);
  delay(1000);
}
