#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 9

#define NUM_LEDS 30

int minBrightness = 10;
int maxBrightness = 80;
int brightness = minBrightness;
bool isBrightening = true;
int drip1 = 16;
int drip2 = 17;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.setBrightness(brightness);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  rainbowSlice(100, 195, 3); // orange
}

void rainbowSlice(uint8_t wait, byte first, byte interval) {

  uint16_t i, j, pixel;

  for (j = 0; j < strip.numPixels(); j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      pixel = i + j;
      if (pixel >= strip.numPixels()) {
        pixel = pixel - strip.numPixels();
      }
      if (pixel == drip1 || pixel == drip2 || pixel == drip1 - 1 || pixel == drip2 - 1) {
        strip.setPixelColor(pixel, strip.Color(0, 0, 0, 255 ) );
      } else {

        strip.setPixelColor(pixel, Wheel((i + first) & 255));
      }
    }

    strip.show();

    if (j % 2 == 0) {

      drip1--;
      if (drip1 < 0) {
        drip1 = 16;
      }

      drip2++;
      if (drip2 >= 30) {
        drip2 = 17;
      }
    }


    if (isBrightening) {
      brightness = brightness + interval;
      if (brightness >= maxBrightness) {
        isBrightening = false;
      }
    } else {
      brightness = brightness - interval;
      if (brightness <= minBrightness) {
        isBrightening = true;
      }
    }
    strip.setBrightness(brightness);
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

