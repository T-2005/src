#include "IR.h"
//Mux control pins
int s0 = 4;
int s1 = 19;
int s2 = 18;
int s3 = 5;

//Mux in "SIG" pin
int SIG_pin = 34;

int IR_3L, IR_4L, IR_5L, IR_6L;
int IR_7C, IR_8C, IR_15C, IR_16C; 
int IR_9R, IR_10R, IR_11R, IR_12R;

void IR :: init(){
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);


}


void IR :: display() 
{

  for(int i = 0; i < 16; i ++){          
    Serial.print(readMux(i));
    Serial.print(" ");
  }
  Serial.println();
  delay(1000);
}


void IR :: detectedline()
{
  int a[6] = {0};
  for(int i = 10; i <= 15; i++)
  {
    if(readMux(i) );
  }
}

// trả về giá trị của các led 
int IR :: ir_3L() {return readMux(3);}
int IR :: ir_4L() {return readMux(4);}
int IR :: ir_5L() {return readMux(5);}
int IR :: ir_6L() {return readMux(6);}

int IR :: ir_7C() {return readMux(7);}
int IR :: ir_8C() {return readMux(8);}
int IR :: ir_15C() {return readMux(15);}
int IR :: ir_16C() {return readMux(16);}

int IR :: ir_9R() {return readMux(9);}
int IR :: ir_10R() {return readMux(10);}
int IR :: ir_11R() {return readMux(11);}
int IR :: ir_12R() {return readMux(12);}
// trả về giá trị của các led

int IR :: readMux(int channel){
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 1
    {1,0,0,0}, //channel 2
    {0,1,0,0}, //channel 3
    {1,1,0,0}, //channel 4
    {0,0,1,0}, //channel 5
    {1,0,1,0}, //channel 6
    {0,1,1,0}, //channel 7
    {1,1,1,0}, //channel 8
    {0,0,0,1}, //channel 9
    {1,0,0,1}, //channel 10
    {0,1,0,1}, //channel 11
    {1,1,0,1}, //channel 12
    {0,0,1,1}, //channel 13
    {1,0,1,1}, //channel 14
    {0,1,1,1}, //channel 15
    {1,1,1,1}  //channel 16
   
   
   
  };

  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  int val = analogRead(SIG_pin); 
  return val;
}