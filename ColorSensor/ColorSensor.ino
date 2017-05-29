// Inspired by https://www.14core.com/wiring-the-tcs320-tcs230-color-detection-with-arduino/
// and https://github.com/MajicDesigns/MD_TCS230
// and http://www.toptechboy.com/arduino/lesson-15-super-cool-arduino-color-sensor-project/

#include <Adafruit_NeoPixel.h>

enum Colors {RED, GREEN, BLUE, CLEAR};

const int sensor = 4;
const int s0 = 5;
const int s1 = 6;
const int s2 = 7;
const int s3 = 8;
const int jewelPin = 9;

Adafruit_NeoPixel jewel = Adafruit_NeoPixel(7, jewelPin, NEO_GRBW + NEO_KHZ800);

unsigned int pulseWidth;
unsigned int redColor;
unsigned int greenColor;
unsigned int blueColor;

unsigned int redLow = 5000;
unsigned int greenLow = 5000;
unsigned int blueLow = 5000;

unsigned int redHigh = 0;
unsigned int greenHigh = 0;
unsigned int blueHigh = 0;

void setup() {
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
  Serial.println("Here we go!");

  // s0 and s1 set the frequency scaling of the reading
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  // s2 and s3 choose the color to read
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  // we read the color from the sensor's output pin
  pinMode(sensor, INPUT);

  // start with no color displayed
  jewel.setBrightness(10);
  jewel.begin();
  jewel.show(); // Initialize all pixels to 'off'

  // calibrate for the first five seconds after program runs
  while (millis() < 15000) {
    // record the min and max values observed for each color
    pulseWidth = readColor(RED);
    if (pulseWidth > redHigh) {
      redHigh = pulseWidth;
    }
    if (pulseWidth < redLow) {
      redLow = pulseWidth;
    }

    pulseWidth = readColor(GREEN);
    if (pulseWidth > greenHigh) {
      greenHigh = pulseWidth;
    }
    if (pulseWidth < greenLow) {
      greenLow = pulseWidth;
    }

    pulseWidth = readColor(BLUE);
    if (pulseWidth > blueHigh) {
      blueHigh = pulseWidth;
    }
    if (pulseWidth < blueLow) {
      blueLow = pulseWidth;
    }
  }

  Serial.print("Red calibration: ");
  Serial.print(redLow);
  Serial.print(" to ");
  Serial.println(redHigh);

  Serial.print("Green calibration: ");
  Serial.print(greenLow);
  Serial.print(" to ");
  Serial.println(greenHigh);

  Serial.print("Blue calibration: ");
  Serial.print(blueLow);
  Serial.print(" to ");
  Serial.println(blueHigh);
}

void loop() {
  // put your main code here, to run repeatedly:
  colorWipe(0, 0, 0, 0);
  delay(100);

  pulseWidth = readColor(RED);
  Serial.print("Red: ");
  Serial.print(pulseWidth);
  redColor = map(pulseWidth, redLow, redHigh, 255, 0);
  Serial.print(" = ");
  Serial.print(redColor);

  pulseWidth = readColor(GREEN);
  Serial.print(", Green: ");
  Serial.print(pulseWidth);
  greenColor = map(pulseWidth, greenLow, greenHigh, 255, 0);
  Serial.print(" = ");
  Serial.print(greenColor);

  pulseWidth = readColor(BLUE);
  Serial.print(", Blue: ");
  Serial.print(pulseWidth);
  blueColor = map(pulseWidth, blueLow, blueHigh, 255, 0);
  Serial.print(" = ");
  Serial.print(blueColor);
  Serial.println(".");

  colorWipe(redColor, greenColor, blueColor, 50);
  delay(500);
}

int readColor(byte Color) {

  switch (Color) {
    case RED:
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      break;

    case GREEN:
      digitalWrite(s2, HIGH);
      digitalWrite(s3, HIGH);
      break;

    case BLUE:
      digitalWrite(s2, LOW);
      digitalWrite(s3, HIGH);
      break;

    case CLEAR:
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      break;
  }

  return pulseIn(sensor, HIGH);
}


// Fill the dots one after the other with a color
void colorWipe(int red, int green, int blue, uint8_t wait) {
  uint32_t color = jewel.Color(red, green, blue);
  for (uint16_t i = 0; i < jewel.numPixels(); i++) {
    jewel.setPixelColor(i, color);
    jewel.show();
    delay(wait);
  }
}



