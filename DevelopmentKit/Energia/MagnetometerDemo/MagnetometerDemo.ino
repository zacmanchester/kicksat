#include <SpriteMag.h>

SpriteMag mag = SpriteMag();

void setup() {
  mag.init();
  Serial.begin(9600);
}

void loop() {
  
  //Values are in Guass
  MagneticField b = mag.read();
  
  Serial.print("x: ");
  Serial.print(b.x);
  Serial.print("    y:");
  Serial.print(b.y);
  Serial.print("    z: ");
  Serial.println(b.z);
  
  delay(250);
}
