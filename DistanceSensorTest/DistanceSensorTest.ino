/*
  Description: sharp 2Y0A710K distance ranger.(Range from 1m to 5.5m.)
  This code prints out data of valid range.
  However, if we put ranger very close to objects, we will have completely wrong results.
  The error is inevitable because of electric character of this device.
  So the only way of correct use is to make sure the distance of objects  not close to the ranger. (1m to 5.5m.)
  Another tip: it is not very precise. So it is fit for detection,but not for measurement.

  Tested by Michael from DFrobot
  2012/2/13
*/

/*
  Principle of this ranger: (See details in datasheet.)
  Voltage is linear to the inverse of distance. (Distance range is 1m to 5.5m)
  Two reference points:
  Analog Voltage(V)   Digital value(0-1023)    Distance(cm)
  2.5                512                     100
  1.4                286                     500
  Then, we have a linear equation of digital value and distance.
  (512-sensorValue)/(1/100-1/distance)=(512-286)/(0.01-0.002) => distance=28250/(sensorValue-229.5);
*/

// Distance sensor reads analog pin 0
const int sensorPin = A0;

// External LED is on digital pin 13
const int ledPin = 13;

// Voltage ranges we care about
const int lowVoltageRange = 286;
const int highVoltageRange = 512;

// Sense within this distance in cm
const int maxReadDistance = 200;

int sensorValue;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {

  int distance = getDistance();

  if (distance < maxReadDistance && distance > 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(200);
}

int getDistance() {

  sensorValue = analogRead(sensorPin);

  // Corresponding distance range from 1m to 5.5m
  if (sensorValue >= lowVoltageRange && sensorValue <= highVoltageRange) {
    Serial.print("The distance is: ");
    distance = 28250 / (sensorValue - 229.5);
    Serial.print(distance);
    Serial.println("cm");
    return distance;
  }
  return 0;
}

