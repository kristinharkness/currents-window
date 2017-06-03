#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

// set to false if using a common cathode LED
#define commonAnode false

const int neopixelPin = 9;
const int numNeopixels = 2;

Adafruit_NeoPixel neopixels = Adafruit_NeoPixel(numNeopixels, neopixelPin, NEO_GRB + NEO_KHZ800);

// our RGB -> eye-recognized gamma color
byte gammatable[256];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  Serial.println("Color View Test!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // start with no color displayed
  neopixels.setBrightness(50);
  neopixels.begin();
  neopixels.show(); // Initialize all pixels to 'off'

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }
}


void loop() {
  uint16_t clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED

  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  Serial.print("\t");
  //Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  //Serial.println();

  Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

  colorWipe(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b], 0);
}

// Fill the dots one after the other with a color
void colorWipe(int red, int green, int blue, uint8_t wait) {
  uint32_t color = neopixels.Color(red, green, blue);
  for (uint16_t i = 0; i < neopixels.numPixels(); i++) {
    neopixels.setPixelColor(i, color);
    neopixels.show();
    delay(wait);
  }
}

