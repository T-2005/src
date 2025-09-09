#include "IR.h"
#include "motor.h"
#include <Arduino.h>

#define PWML 13
#define in1L 14
#define in2L 12
#define PWMR 32
#define in1R 33
#define in2R 25


// parameter of PID
float Kp = 1.2;
float Ki = 0.08;
float Kd = 0.05;
float setpoint = 0.0;
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
  IR.write_information(9); // in giá trị so sánh và giá trị của 1 ir cụ thể

   
 }


float PID_value()
{
 
   float readValue = 0.0;
  if(IR.detectedline()) readValue = 0.0;
  else if(IR.ir_6L()) readValue = 25.0;
  else if(IR.ir_5L()) readValue = 50.0;
  else if( IR.ir_4L()) readValue = 75.0;
  else if(IR.ir_3L()) readValue = 100.0;
  
  else if(IR.ir_9R()) readValue = -25.0;
  else if( IR.ir_10R()) readValue =  -50.0;
  else if(IR.ir_11R()) readValue = -75.0;
  else if( IR.ir_12R()) readValue = -100.0;
  Serial.print("readValue: "); Serial.println(readValue);
     float sp = robot.PID_control(setpoint, readValue, lastTimePID, Kp, Ki, Kd);
     lastTimePID = millis();
  return sp;
}

void followline()
{
  float PID = PID_value();
  int rsp = (int) (speed - PID);
  int lsp = (int) (speed + PID);

  if (lsp > 250) {
    lsp = 250;
  }
  if (lsp < 0) {
    lsp = 50;
  }
  if (rsp > 250) {
    rsp = 250;
  }
  if (rsp < 0) {
    rsp = 50;
  }
  Serial.print("lsp: "); Serial.println(lsp);
  Serial.print("rsp: "); Serial.println(rsp);
  if(IR.fullline() == 1) 
   {
     robot.stop();
    Serial.println("stop!");
   }
  else 
  {
    robot.motion(lsp, rsp);
    Serial.println("activing!");
  }
 
}
void loop() {
 IR.write_information(9); // in giá trị so sánh và giá trị của 1 ir cụ thể
  IR.write_line(); // hàm trả về giá trị nhị phân 0 - 1 tương ứng vs từng mắt IR đang nằm trên line trắng hoặc đen
   
  
  ///robot.motion(250, 250);

   followline();
   Serial.println();
  delay(2000);
}