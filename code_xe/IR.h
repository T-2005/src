#ifndef IR__H
#define IR__H
#include <Arduino.h>
extern int s0;
extern int s1;
extern int s2;
extern int s3;
extern int SIG_pin;

extern int IR_3L, IR_4L, IR_5L, IR_6L;
extern int IR_7C, IR_8C, IR_15C, IR_16C; 
extern int IR_9R, IR_10R, IR_11R, IR_12R;
class IR
{
  public:
  static void init();
  static void display(); // in ra gia tri 16 mat line
  static int readMux(int channel); // tra ve gia tri cua tung led 0 - > 16
  static void detectedline();
};
#endif