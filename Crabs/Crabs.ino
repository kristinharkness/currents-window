#include <Servo.h>

typedef struct {
  int servoPin;
  int low;
  int high;
  int pos;
  bool increasing;
} Crab;

Crab blueCrab = {9, 15, 50, 15, true};
Servo blueCrabServo;

Crab pinkCrab = {10, 10, 50, 10, true};
Servo pinkCrabServo;

const int servoDelay = 50; // the shorter the delay the faster the crab moves

void setup() {
  Serial.begin(9600);
  blueCrabServo.attach(blueCrab.servoPin);
  pinkCrabServo.attach(pinkCrab.servoPin);
}

void loop() {
  blueCrabServo.write(blueCrab.pos);
  pinkCrabServo.write(pinkCrab.pos);
  blueCrab = nextCrabPos(blueCrab);
  pinkCrab = nextCrabPos(pinkCrab);
  delay(servoDelay);
}

Crab nextCrabPos(Crab crab) {
  if (crab.increasing) {
    crab.pos++;
    if (crab.pos == crab.high) {
      crab.increasing = false;
    }
  } else {
    crab.pos--;
    if (crab.pos == crab.low) {
      crab.increasing = true;
    }
  }
  Serial.println(crab.pos);
  return crab;
}

