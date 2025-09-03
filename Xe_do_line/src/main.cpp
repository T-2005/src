#include "IR.h"
#include "motor.h"
#include <Arduino.h>

#define PWML 13
#define in1L 14
#define in2L 12
#define PWMR 32
#define in1R 26
#define in2R 25

// parameter of PID
float Kp = 0.3;
float Ki = 0.1;
float Kd = 0.1;
float setpoint = 250;
uint32_t lastTimePID = 0.0;
float PID_outPut = 0.0;
// parameter of PID

int speed = 250;
motor robot(PWML, in1L, in2L, PWMR, in1R, in2R);
IR IR;
void setup() {
  Serial.begin(115200);
  IR.init();

  // đọc giá trị line trắng và đen để tạo giá trị so sánh 
  IR.read_black_line();
  IR.read_white_line();
  // đọc giá trị line trắng và đen để tạo giá trị so sánh 

  IR.write_information(16); // in giá trị so sánh và giá trị của 1 ir cụ thể
}


float PID_value()
{
  float readValue = 0.0;
  if(IR.detectedline()) readValue = 0.0;
  if(IR.detectedline() && (IR.IR_56() || IR.IR_910())) readValue = 50.0;
  if(IR.detectedline() && (IR.IR_34() || IR.IR_1112())) readValue = 150.0;
    float sp = robot.PID_control(setpoint, readValue, lastTimePID, Kp, Ki, Kd);
  return sp;
}

void followline()
{
  int PID = PID_value();
  int lsp = speed - PID;
  int rsp = speed + PID;

  if (lsp > 250) {
    lsp = 250;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 250) {
    rsp = 250;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  robot.motion(lsp, rsp);
}
void loop() {
 
  IR.write_line(); // hàm trả về giá trị nhị phân 0 - 1 tương ứng vs từng mắt IR đang nằm trên line trắng hoặc đen
  
  

  followline();
  Serial.println();
  delay(2000);
}