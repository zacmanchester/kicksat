/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This code is for the Sprite development kit board
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
