#include <Servo.h>  //load servo library
int pos = 0;   //declare and initialize the pos variable
int servoPin = 9; //tell Arduino that servo is hooked to pin 9
int servoDelay = 25; //servo delay

Servo myPointer; //create a servo object called myPointer

void setup() {

  Serial.begin(9600);     //turn on serial port
  myPointer.attach(servoPin); //declare that myPoint servo is attached to the arduino at servoPin


}

void loop() {

  for (pos = 10; pos <= 50; pos = pos + 1) {

    myPointer.write(pos);     //send servo to pos
    delay(servoDelay);      //pause a moment

  }

  for (pos = 50; pos >= 10; pos = pos - 1) {

    myPointer.write(pos);     //send servo to pos
    delay(servoDelay);      //pause a moment

  }

}
