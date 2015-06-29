#include <Servo.h>

Servo servo;

const int buttonPin = 2;
const int motorPin = 3;
const int servoPin = 9;
const int IRpin = 0;                                    // analog pin for reading the IR sensor
const int IRpin2 = 3;                                    // analog pin for reading the IR sensor

int buttonState =LOW;      // the current state of the output pin
int current;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
int motorspeed = 100;
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);
  servo.attach(servoPin);
  Serial.begin(9600);                             // start the serial port
}

void loop() {
  int i=0;
  int distance = analogRead(IRpin);
  int distance2 = analogRead(IRpin2);
  
  Serial.print(distance);
  Serial.print(" ");
  Serial.println(distance2);
  
  int angle = map(distance, 0, 1023, 0, 179);
  servo.write(angle);
  
  current = digitalRead(buttonPin);
  
  if (current == HIGH && previous == LOW && millis() - time > debounce) {
    if (buttonState == HIGH)
    {
      buttonState = LOW;
      analogWrite(motorPin,0);  //analogWrite for PWM signals
    }
    else
    { 
      buttonState = HIGH;
      time = millis();  
      analogWrite(motorPin,90);  //analogWrite PWM signals
    }
  }
  previous = current;
  delay(200);
}
