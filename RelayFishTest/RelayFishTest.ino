
typedef struct {
  int relayPin;
  int sensorPin;
  long lastSenseTime;
  long duration;
} Region;

int numRegions = 1;
Region regions[] = {{2, A0, 0, 2000}, {3, A1, 0, 2000}};

// Distance sensor voltage ranges we care about
const int lowVoltageRange = 286;
const int highVoltageRange = 512;

// Sense within this distance in cm
const int maxReadDistance = 200;

void setup() {
  Serial.begin(9600);
  //numRegions = sizeof(regions) / sizeof(regions[0]); // use runtime size

  // HIGH means the relay is open (off)
  for (int i = 0; i < numRegions; i++) {
    digitalWrite(regions[i].relayPin, HIGH);
    pinMode(regions[i].relayPin, OUTPUT);
  }
}

void loop() {

  for (int i = 0; i < numRegions; i++) {
    int distance = getDistance(regions[i].sensorPin);
    if (distance > 0) {
      regions[i].lastSenseTime = millis();
      digitalWrite(regions[i].relayPin, LOW);
    } else {
      long now = millis();
      Serial.print("Now: ");
      Serial.print(now);
      Serial.print(" last sense time ");
      Serial.println(regions[i].lastSenseTime);
      if (now - regions[i].lastSenseTime > regions[i].duration) {
        digitalWrite(regions[i].relayPin, HIGH);
        regions[i].lastSenseTime = 0;
        Serial.println("turning off");
      }
    }
  }
  delay(1000);
}

int getDistance(int sensorPin) {

  int sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);

  // Corresponding distance range from 1m to 5.5m
  if (sensorValue >= lowVoltageRange && sensorValue <= highVoltageRange) {
    Serial.print("The distance on pin ");
    Serial.print(sensorPin);
    Serial.print(" is: ");
    int distance = 28250 / (sensorValue - 229.5);
    Serial.print(distance);
    Serial.println(" cm");
    if (distance <= maxReadDistance) {
      return distance;
    }
  }
  return 0;
}
