#include <B_LUX_V30.h> 
B_LUX_V30 lux(2, 3);//SDA=2; SCL=3  
void setup() {
  Serial.begin(9600);
  while (!Serial) ;;;
  Serial.print("OK!");
}

void loop() {
	Serial.println(lux.read());
	delay(1000);
}

