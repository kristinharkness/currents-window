


void setup() {
  for (int i = 2; i <= 6; i++) {
    digitalWrite(i, HIGH);
    pinMode(i, OUTPUT);
  }
}

void loop() {
  for (int i = 2; i <= 6; i++) {
    digitalWrite(i, LOW);
    delay(500);
    digitalWrite(i, HIGH);
    delay(500);
  }
}
