#include "IR.h"
IR IR;
void setup() {
  Serial.begin(115200);
  IR.init();
  IR.read_black_line();
  
  IR.read_white_line();
 
 IR.read_information(16, 8);
}

void loop() {
 
  
 Serial.println();
  delay(2000);

}
