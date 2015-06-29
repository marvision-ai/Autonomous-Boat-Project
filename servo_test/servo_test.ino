#include <Servo.h>

Servo servo;

const int servoPin = 9;

void setup() {
  servo.attach(servoPin);
}

void loop() {
//    servo.write(30);
    delay(1000);
    servo.write(90);
    delay(500);
    servo.write(91);
    delay(500);
    servo.write(92);
    delay(500);
    servo.write(93);
    delay(500);
    servo.write(94);
    delay(500);
    servo.write(95);
    delay(500);
    servo.write(96);
    delay(500);
    servo.write(97);
    delay(500);
    servo.write(98);
    delay(500);
    servo.write(99);
    delay(500);
    servo.write(100);
//    delay(1000);
//    servo.write(150);
//    delay(1000);    
}
