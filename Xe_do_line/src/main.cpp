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
float Kp = 2.4;
float Ki = 0.08;
float Kd = 0.05;
float setpoint = 0.0;
uint32_t lastTimePID = 0.0;
float PID_outPut = 0.0;
// parameter of PID

int _led = 27;
int touch = 4;
int speed = 150;

int dem = 0;
motor robot(PWML, in1L, in2L, PWMR, in1R, in2R);
IR IR;

void setup()
{
  Serial.begin(115200);
  pinMode(_led, OUTPUT);
  IR.init();

  
  while (dem < 4)
  {
    
    if (touchRead(touch) < 25) 
    {
      if (dem == 0)
      {
        IR.read_black_line();
        digitalWrite(_led , HIGH);
        delay(200);
        digitalWrite(_led, LOW);
        dem = 1;
      } 
      else if (dem == 1)
      {
        IR.read_white_line();
        digitalWrite(_led , HIGH);
        delay(200);
        digitalWrite(_led, LOW);
        dem = 2;
      }
      else if (dem == 2)
      {
        IR.write_information(6);
        digitalWrite(_led , HIGH);
        delay(200);
        digitalWrite(_led, LOW);
        dem = 3; 
      }
      else if(dem == 3) 
      {
        
        digitalWrite(_led , HIGH);
        delay(200);
        digitalWrite(_led, LOW);
        dem = 4;
      }
      delay(200); 
    }
  }
}

float PID_value()
{

  float readValue = 0.0;
  // readValue = (float)(IR.ir_6L() * 5.0) + (float)(IR.ir_5L() * 10.0) + (float)(IR.ir_4L() * 15.0) + (float)(IR.ir_3L() * 20.0) + (float)(IR.ir_2L() * 25.0) + (float)(IR.ir_1L() * 30.0);
  // readValue += (float)(IR.ir_9R() * -5.0) + (float)(IR.ir_10R() * -10.0) + (float)(IR.ir_11R() * -15.0) + (float)(IR.ir_12R() * -20.0) + (float)(IR.ir_13R() * -25.0) + (float)(IR.ir_14R() * -30.0);

  readValue = (float)(IR.ir_6L() * 5.0) + (float)(IR.ir_5L() * 7.5) + (float)(IR.ir_4L() * 10.0) + (float)(IR.ir_3L() * 12.5) + (float)(IR.ir_2L() * 15.0) + (float)(IR.ir_1L() * 17.5);
  readValue += (float)(IR.ir_9R() * -5.0) + (float)(IR.ir_10R() * -7.5) + (float)(IR.ir_11R() * -10.0) + (float)(IR.ir_12R() * -12.5) + (float)(IR.ir_13R() * -15.0) + (float)(IR.ir_14R() * -17.5);

  Serial.print("readValue: ");
  Serial.println(readValue);
  float sp = robot.PID_control(setpoint, readValue, lastTimePID, Kp, Ki, Kd);
  lastTimePID = millis();
  return sp;
}

void followline()
{
  float PID = PID_value();
  int lsp = (int)(speed - PID);
  int rsp = (int)(speed + PID);

  if (lsp > speed)
  {
    lsp = speed;
  }
  if (lsp < 0)
  {
    lsp = 0;
  }
  if (rsp > speed)
  {
    rsp = speed;
  }
  if (rsp < 0)
  {
    rsp = 0;
  }
  Serial.print("lsp: ");
  Serial.println(lsp);
  Serial.print("rsp: ");
  Serial.println(rsp);
  if (IR.fullline() == 1)
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
void loop()
{
  //  IR.write_information(6); // in giá trị so sánh và giá trị của 1 ir cụ thể
  //   IR.write_line(); // hàm trả về giá trị nhị phân 0 - 1 tương ứng vs từng mắt IR đang nằm trên line trắng hoặc đen

  //robot.motion(250, 250);

   while(dem == 4) 
   {
     followline();
   }
  Serial.println();
}