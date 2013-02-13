#include <SpriteMag.h>
#include <SpriteGyro.h>

SpriteMag mag = SpriteMag();
SpriteGyro gyro = SpriteGyro();

void setup() {
  gyro.init();
  mag.init();
  Serial.begin(9600);
}

void loop() {
  
  //Values are in Guass
  MagneticField b = mag.read();
  Serial.print("Bx: ");
  Serial.print(b.x);
  Serial.print("    By:");
  Serial.print(b.y);
  Serial.print("    Bz: ");
  Serial.println(b.z);
  
  AngularVelocity w = gyro.read();
  Serial.print("Wx: ");
  Serial.print(w.x);
  Serial.print("    Wy:");
  Serial.print(w.y);
  Serial.print("    Wz: ");
  Serial.println(w.z);
  
  Serial.println("");
  
  delay(250);
}
