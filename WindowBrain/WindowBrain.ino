#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

typedef struct {
  int relayPin;
  int sensorPin;
  bool lastReadingInRange; // we want at least 2 readings in a row before we decide it's positive
  long lastCloseTime;
  long relayCloseDuration;
} Region;

int numRegions = 4;
Region regions[] = {
  {2, A0, false, 0, 2000},  // Squidoo / Yellow Submarine
  {4, A1, false, 0, 2000},  //
  {5, A2, false, 0, 20000}, // Starfish
  {6, A3, false, 0, 2000}   //
};

// The brain also sets Squidoo's color
const int squidPin = 9;
const int squidNumLeds = 30;
int squidBrightness = 40;
Adafruit_NeoPixel squid = Adafruit_NeoPixel(squidNumLeds, squidPin, NEO_GRB + NEO_KHZ800);

// Distance sensor voltage ranges we care about
const int lowVoltageRange = 286;
const int highVoltageRange = 512;

// Sense within this distance in cm
const int maxReadDistance = 200;

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

  // Initialize Squid - start with all pixels 'off'
  squid.setBrightness(squidBrightness);
  uint32_t off = squid.Color(0, 0, 0);
  for (int i = 0; i < squid.numPixels(); i++) {
    squid.setPixelColor(off, 0);
  }
  squid.begin();
  squid.show();
}

void loop() {

  for (int i = 0; i < numRegions; i++) {
    now = millis();
    distance = getDistance(regions[i].sensorPin);

    Serial.print(i);
    Serial.print(" Distance: "); Serial.print(distance); Serial.print(" Now: "); Serial.print(now);
    Serial.print(" last close time "); Serial.print(regions[i].lastCloseTime);
    Serial.print(" last reading in range "); Serial.println(regions[i].lastReadingInRange);

    // We sensed something in range
    if (distance > 0) {

      // If we're in the duration timer, it starts over
      if (regions[i].lastCloseTime > 0) {
        digitalWrite(regions[i].relayPin, LOW);
        regions[i].lastCloseTime = now;
      }

      // If this is our first in range reading, mark that
      else if (!regions[i].lastReadingInRange)  {
        regions[i].lastReadingInRange = true;
      }

      // if this is a subsequent in range reading, close the relay
      else {
        digitalWrite(regions[i].relayPin, LOW);
        regions[i].lastCloseTime = now;
        regions[i].lastReadingInRange = false;
        Serial.print("turning on"); Serial.println(i);
      }

      if (i == 0) {
        doSquidColor(distance);
      }
    }

    // We did not sense anything in range
    else {
      // Reset our in range flag
      regions[i].lastReadingInRange = false;

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
    delay(5);
  }
}

int getDistance(int sensorPin) {

  int sensorValue = analogRead(sensorPin);
  // Serial.println(sensorValue);

  // Corresponding distance range from 1m to 5.5m
  if (sensorValue >= lowVoltageRange && sensorValue <= highVoltageRange) {
    int distance = 28250 / (sensorValue - 229.5);
    // Serial.print("The distance on pin "); Serial.print(sensorPin); Serial.print(" is: "); Serial.print(distance); Serial.println(" cm");
    if (distance <= maxReadDistance) {
      return distance;
    }
  }
  return 0;
}

void doSquidColor(int distance) {
  uint32_t newColor = Wheel(map(distance, 0, maxReadDistance, 0, 255));
  for (int i = 0; i < squid.numPixels(); i++) {
    squid.setPixelColor(newColor, 0);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return squid.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return squid.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return squid.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

