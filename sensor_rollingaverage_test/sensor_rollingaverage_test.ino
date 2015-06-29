const int sideIRpin = 3;
const int frontIRpin = 3;

long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

const int numReadings = 6;

int sideReadings[numReadings];
int sideIndex = 0;
int sideTotal = 0;
int sideAverage = 0;
int frontReadings[numReadings];
int frontIndex = 0;
int frontTotal = 0;
int frontAverage = 0;

void setup() {
  // initialize readings in the running average distance to 0
  for (int reading = 0; reading < numReadings; reading++) {
    sideReadings[reading] = 0;
    frontReadings[reading] = 0;
  }
  Serial.begin(9600);                             // start the serial port
}

void loop() {
  while (millis() < 30000) {
    addSideDistance();
//    addFrontDistance();
  
    Serial.println(sideAverage);
//    Serial.print(" ");
//    Serial.println(frontAverage);
    
    delay(10);  
  }
}

void addSideDistance() {
  sideTotal = sideTotal - sideReadings[sideIndex];
  sideReadings[sideIndex] = analogRead(sideIRpin);
  Serial.print(sideReadings[sideIndex]);
  Serial.print(" ");
  sideTotal = sideTotal + sideReadings[sideIndex];
  sideIndex = sideIndex + 1;

  if (sideIndex >= numReadings) {
    sideIndex = 0;
  }

  sideAverage = sideTotal / numReadings;
}

void addFrontDistance() {
  frontTotal = frontTotal - frontReadings[frontIndex];
  frontReadings[frontIndex] = analogRead(frontIRpin);
  Serial.print(frontReadings[frontIndex]);
  Serial.print(" ");
  frontTotal = frontTotal + frontReadings[frontIndex];
  frontIndex = frontIndex + 1;

  if (frontIndex >= numReadings) {
    frontIndex = 0;
  }

  frontAverage = frontTotal / numReadings;
}
