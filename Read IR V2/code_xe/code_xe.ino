#include "IR.h"
IR IR;
void setup() {
  Serial.begin(115200);
  IR.init();

  // // đọc giá trị line trắng và đen để tạo giá trị so sánh 
  // IR.read_black_line();
  // IR.read_white_line();
  // // đọc giá trị line trắng và đen để tạo giá trị so sánh 

  // IR.write_information(16, 8); // in giá trị so sánh và giá trị của 1 ir cụ thể
}

void loop() {
 
 for(int i = 1; i <= 16; i++)
 {
  Serial.print(IR.aver(i)); Serial.print("|");
 }
 
 Serial.println();
  delay(2000);

}
