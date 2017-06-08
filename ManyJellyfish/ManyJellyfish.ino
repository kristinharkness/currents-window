#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const int numLeds = 30;

typedef struct {
  int pin;
  byte first;
  byte interval;
  int minBrightness;
  int maxBrightness;
  int brightness;
  bool isBrightening;
  int drip1;
  int drip2;
  Adafruit_NeoPixel strip;
} Jellyfish;

Jellyfish jellies[] = {
  {9, 253, 3, 30, 10, 80, 10, true, 16, 17}
};

void setup() {
  Serial.begin(9600);
  jellies[0].strip = Adafruit_NeoPixel(numLeds, jellies[0].pin, NEO_GRB + NEO_KHZ800);
  jellies[0].strip.setBrightness(jellies[0].brightness);
  jellies[0].strip.begin();
  jellies[0].strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  //rainbowSlice(100, 90, 180); // blue
  //rainbowSlice(100, 170, 255); // pink
  rainbowSlice(100); // orange
}

void rainbowSlice(uint8_t wait) {

  uint16_t i, j, pixel;

  for (j = 0; j < numLeds; j++) {
    for (i = 0; i < numLeds; i++) {
      
      pixel = i + j;
      if (pixel >= numLeds) {
        pixel = pixel - numLeds;
      }

      for (int idx = 0; idx < 1; idx++) {
        
        Jellyfish jelly = jellies[idx];
        
        if (pixel == jelly.drip1 || pixel == jelly.drip2 || pixel == jelly.drip1 - 1 || pixel == jelly.drip2 - 1) {
          jelly.strip.setPixelColor(pixel, jelly.strip.Color(0, 0, 0, 255 ));
        } else {
          jelly.strip.setPixelColor(pixel, Wheel((i + jelly.first) & 255));
        }


        jelly.strip.show();

        if (j % 2 == 0) {

          jelly.drip1--;
          if (jelly.drip1 < 0) {
            jelly.drip1 = 16;
          }

          jelly.drip2++;
          if (jelly.drip2 >= 30) {
            jelly.drip2 = 17;
          }
        }

        if (jelly.isBrightening) {
          jelly.brightness = jelly.brightness + jelly.interval;
          if (jelly.brightness >= jelly.maxBrightness) {
            jelly.isBrightening = false;
          }
        } else {
          jelly.brightness = jelly.brightness - jelly.interval;
          if (jelly.brightness <= jelly.minBrightness) {
            jelly.isBrightening = true;
          }
        }

        jelly.strip.setBrightness(jelly.brightness);
      }
    }
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return jellies[0].strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return jellies[0].strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return jellies[0].strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}



