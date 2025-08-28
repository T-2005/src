#include "IR.h"
IR IR;
void setup() {
  Serial.begin(115200);
  IR.init();

}

void loop() {
  for(int i = 10; i <= 16; i++)
  {
    float tmp = IR.readMux(i);
   Serial.print(tmp);
   Serial.print(" ");

  }
  
  Serial.println();
  delay(2000);

}
