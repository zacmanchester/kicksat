/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

void setup() {                
  pinMode(5, OUTPUT);
}

void loop() {
  digitalWrite(5, HIGH);   // set the LED on
  delay(500);
  digitalWrite(5, LOW);    // set the LED off
  delay(500);
}
