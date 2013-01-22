#include <SpriteGyro.h>

SpriteGyro gyro = SpriteGyro();

void setup() {
  gyro.init();
  Serial.begin(9600);
}

void loop() {
  
  AngularVelocity w = gyro.read();
  
  Serial.print("x: ");
  Serial.print(w.x);
  Serial.print("    y:");
  Serial.print(w.y);
  Serial.print("    z: ");
  Serial.println(w.z);
  
  delay(250);
}
