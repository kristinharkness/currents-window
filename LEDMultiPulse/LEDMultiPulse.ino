
// Modified from https://www.sparkfun.com/tutorials/329

typedef struct {
  int pin;
  float in;
  float start;
} Fish;

const float wavelength = 6.283;

Fish fishes[] = {{3, 4.712, 4.712}, {5, 0, 0}, {6, 2.3, 2.3}};
int numFishes = 3;

void setup() {
  // we set numFishes above, but be sure that is right ...
   numFishes = (sizeof(fishes) / sizeof(fishes[0]));
}

void loop() {
  // do input, etc. here - as long as you don't pause, the LED will keep pulsing
  float out;
  for (int i = 0; i < numFishes; i++) {
    fishes[i].in = fishes[i].in + 0.001;
    if (fishes[i].in > fishes[i].start + wavelength)
      fishes[i].in - fishes[i].start;
    out = sin(fishes[i].in) * 127.5 + 127.5;
    analogWrite(fishes[i].pin, out);
    fishes[i].in = fishes[i].in + .001 * out / 255;
  }
  //delay(1);
}
