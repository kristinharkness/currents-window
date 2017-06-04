
typedef struct {
  int relayPin;
  int sensorPin;
  long firstSenseTime;
  long lastCloseTime;
  long relayCloseDuration;
} Region;

int numRegions = 3;
Region regions[] = {{2, A0, 0, 0, 2000}, {4, A1, 0, 0, 2000}, {5, A2, 0, 0, 2000}};

// Distance sensor voltage ranges we care about
const int lowVoltageRange = 286;
const int highVoltageRange = 512;

// Sense within this distance in cm
const int maxReadDistance = 200;
const long sensorDuration = 25;

long now;
int distance;

void setup() {
  Serial.begin(9600);
  numRegions = sizeof(regions) / sizeof(regions[0]); // use runtime size

  // HIGH means the relay is open (off)
  for (int i = 0; i < numRegions; i++) {
    digitalWrite(regions[i].relayPin, HIGH);
    pinMode(regions[i].relayPin, OUTPUT);
  }
}

void loop() {

  for (int i = 0; i < numRegions; i++) {
    now = millis();
    distance = getDistance(regions[i].sensorPin);

    Serial.print(i);
    Serial.print(" Distance: "); Serial.print(distance); Serial.print(" Now: "); Serial.print(now);
    Serial.print(" last close time "); Serial.print(regions[i].lastCloseTime);
    Serial.print(" first sense time "); Serial.println(regions[i].firstSenseTime);

    // We sensed something in range
    if (distance > 0) {

      // If we're in the duration timer, it starts over
      if (regions[i].lastCloseTime > 0) {
        digitalWrite(regions[i].relayPin, LOW);
        regions[i].lastCloseTime = now;
      }

      // If this is our first in range reading, start our duration measure
      else if (regions[i].firstSenseTime == 0)  {
        regions[i].firstSenseTime = now;
      }

      // If we've had an in range reading for long enough, close the relay
      else if (now - regions[i].firstSenseTime > sensorDuration) {
        digitalWrite(regions[i].relayPin, LOW);
        regions[i].lastCloseTime = now;
        regions[i].firstSenseTime = 0;
        Serial.print("turning on"); Serial.println(i);
      }
    }

    // We did not sense anything in range
    else {
      // Reset our sensor duration measure
      regions[i].firstSenseTime = 0;

      // Are we in the wait period while the relay is closed?
      if (regions[i].lastCloseTime > 0) {

        // Have we waited long enough?
        if (now - regions[i].lastCloseTime > regions[i].relayCloseDuration) {
          digitalWrite(regions[i].relayPin, HIGH);
          regions[i].lastCloseTime = 0;
          Serial.println("turning off"); Serial.println(i);
        }
      }
      // We did not sense anything and we're not waiting so relay should be open
      else {
        digitalWrite(regions[i].relayPin, HIGH);
        regions[i].lastCloseTime = 0;
      }
    }
    //delay(5);
  }
}

int getDistance(int sensorPin) {

  int sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);

  // Corresponding distance range from 1m to 5.5m
  if (sensorValue >= lowVoltageRange && sensorValue <= highVoltageRange) {
    int distance = 28250 / (sensorValue - 229.5);
    //Serial.print("The distance on pin "); Serial.print(sensorPin); Serial.print(" is: "); Serial.print(distance); Serial.println(" cm");
    if (distance <= maxReadDistance) {
      return distance;
    }
  }
  return 0;
}
