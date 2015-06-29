const int sideIRpin = 0;
const int frontIRpin = 3;

long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup() {
  Serial.begin(9600);                             // start the serial port
}

void loop() {
  while (millis() < 3000) {
    int side = analogRead(sideIRpin);
  
    Serial.println(side);
  
    delay(10);  
  }
}
