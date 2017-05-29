
// Modified from https://www.sparkfun.com/tutorials/329

const int LED = 11;

void setup() {
  // put your setup code here, to run once:
}

void loop()
{
  static float in = 4.712;
  float out;
 
 // do input, etc. here - as long as you don't pause, the LED will keep pulsing
  
  in = in + 0.001;
  if (in > 10.995)
    in = 4.712;
  out = sin(in) * 127.5 + 127.5;
  analogWrite(LED,out);
  in = in + .001 * out / 255;
  //delay(1);
}
