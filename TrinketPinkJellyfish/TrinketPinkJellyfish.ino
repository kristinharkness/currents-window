#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 4

#define NUM_LEDS 30
#define MAX_LEDS 90

const int minBrightness = 10;
const int maxBrightness = 100;
const int first = 195;    // The color to start with
const int pulseSpeed = 3; // how fast the brightness pulses: higher = faster
const int dripSpeed = 2;  // how fast the drips move: higher
const int wait = 100;     // how slow the pixels move - higher = slower

int brightness = minBrightness;
bool isBrightening = true;
int drip1 = 16;
int drip2 = 17;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  strip.setBrightness(brightness);
  strip.begin();
  colorWipe(strip.Color(0, 0, 0, 255), 0); // White
}

void loop() {

  uint16_t i, j, pixel;

  for (j = 0; j < strip.numPixels(); j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      pixel = i + j;
      if (pixel >= strip.numPixels()) {
        pixel = pixel - strip.numPixels();
      }
      if (pixel == drip1 || pixel == drip2 || pixel == drip1 - 1 || pixel == drip2 - 1) {
        strip.setPixelColor(pixel, strip.Color(0, 0, 0, 255 ));
      } else {
        strip.setPixelColor(pixel, Wheel((i + first) & 255));
      }
    }

    strip.show();

    if (j % dripSpeed == 0) {

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
      brightness = brightness + pulseSpeed;
      if (brightness >= maxBrightness) {
        isBrightening = false;
      }
    } else {
      brightness = brightness - pulseSpeed;
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
  for (uint16_t i = 0; i < MAX_LEDS; i++) {
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

