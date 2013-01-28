/*
  Blink
  Turns on an LEDs on the Sprite souvenir in sequence
 */

void setup() {                
  // initialize the digital pins as outputs.
  // Pins 1-6 have LEDs connected on the Sprite Replica boards:
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  digitalWrite(1, HIGH);   // set the LED above the "i" on
}

void loop() {
  //Turn LEDs on in sequence
  digitalWrite(6, HIGH);   // set the LED on
  delay(300);
  digitalWrite(5, HIGH);   // set the LED on
  delay(300);
  digitalWrite(4, HIGH);   // set the LED on
  delay(300);
  digitalWrite(3, HIGH);   // set the LED on
  delay(300);
  digitalWrite(2, HIGH);   // set the LED on
  delay(600);
  digitalWrite(2, LOW);    // set the LED off
  digitalWrite(3, LOW);    // set the LED off
  digitalWrite(4, LOW);    // set the LED off
  digitalWrite(5, LOW);    // set the LED off
  digitalWrite(6, LOW);    // set the LED off
  delay(600);
}
