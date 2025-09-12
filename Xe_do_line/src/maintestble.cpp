// update code: code chức năng lùi cho 2 bánh trong hàm motion
// code chạy fullmap



#include "IR.h"
#include "motor.h"
#include <Arduino.h>
#include "BLE.h"

#define PWML 13
#define in1L 14
#define in2L 12
#define PWMR 32
#define in1R 33
#define in2R 25

unsigned char last_turn = 0;
// parameter of PID

// float Kp = 5.6;
// float Ki = 0;
// float Kd = 0.8;

float Kp = 28;
float Ki = 1.2 * Kp * 0.1;
float Kd = 0.075 * Kp * 0.1;

float setpoint = 0.0;
uint32_t lastTimePID = 0.0;
float PID_outPut = 0.0;

int _led = 27; 
int touch = 4;
int speed = 255;

int _buzzer = 26;
int buz_channel = 0;
int dem = 0;

motor robot(PWML, in1L, in2L, PWMR, in1R, in2R);

void blink(){
  // nháy __led, nhấp nháy đến khi touch
  digitalWrite(_led, HIGH);
  delay(100);
  digitalWrite(_led, LOW);
  delay(100);
  //QB
}

// void takeline(){
//   // lấy mẫu line
//   Serial.println("Taking lines started!!");   
//   pinMode(_led, OUTPUT);
//   while (touchRead(4) < 20) blink();
//   IR.read_black_line();

//   while (touchRead(4) < 20) blink();
//   IR.read_white_line();

//   while (touchRead(4) < 20) blink();
//   Serial.println("Take lines done!");
//   //QB
// }

void setup()
{
    Serial.begin(115200);
    // Tên thiết bị Bluetooth
    pinMode(_led, OUTPUT);
    IR.init();
    BLE.setBLE();
    Serial.println("done init IR");
    // gửi thông báo qua Bluetooth
    while (dem < 3)
    {
        if (touchRead(touch) < 25)
        {
            digitalWrite(_led, HIGH);
            delay(100);
            digitalWrite(_led, LOW);
            if (dem == 0)
            {
                Serial.println("doc line den");              
                IR.read_black_line(); // đọc line đen 
                digitalWrite(_led, HIGH);
                delay(100);
                digitalWrite(_led, LOW);
                dem = 1;
            }
            else if (dem == 1)
            {
                Serial.println("doc line trang");
                IR.read_white_line(); // đọc line trắng 
                digitalWrite(_led, HIGH);
                delay(100);
                digitalWrite(_led, LOW);
                dem = 2;
            }
            else if (dem == 2)
            {             
                while (touchRead(touch) > 25)
                {
                    BLE.print_line_BT(); // in qua bluetooth
                    IR.print_line();    // in qua Serial
                    digitalWrite(_led, HIGH);
                    delay(100);
                    digitalWrite(_led, LOW);
                }
                dem = 3;
            }           
            delay(200);
        }
    }
}

// void setup()
// {
//     Serial.begin(115200);
//     // Tên thiết bị Bluetooth
//     pinMode(_led, OUTPUT);
//     IR.init();
//     takeline();

//     Serial.println("done init IR");
//     ledcAttachPin(_buzzer, buz_channel);
//     // gửi thông báo qua Bluetooth
// }

void volume(){
    ledcWriteTone(buz_channel, 1000);
    delay(1000);
}
float PID_value()
{

    float readValue = 0.0;
    readValue = 0.6 * (IR.ir_6L() - IR.ir_9R()) + 1.2 * (IR.ir_5L() - IR.ir_10R()) + 3.1 * (IR.ir_4L() - IR.ir_11R()) + 4.3 * (IR.ir_3L() - IR.ir_12R()) + 5.5 * (IR.ir_2L() - IR.ir_13R()) + 6.6 * (IR.ir_1L() - IR.ir_14R());

    // readValue = (float)(IR.ir_6L() * 5.0) + (float)(IR.ir_5L() * 10.0) + (float)(IR.ir_4L() * 15.0) + (float)(IR.ir_3L() * 20.0) + (float)(IR.ir_2L() * 25.0) + (float)(IR.ir_1L() * 30.0);
    // readValue += (float)(IR.ir_9R() * -5.0) + (float)(IR.ir_10R() * -10.0) + (float)(IR.ir_11R() * -15.0) + (float)(IR.ir_12R() * -20.0) + (float)(IR.ir_13R() * -25.0) + (float)(IR.ir_14R() * -30.0);

    // readValue = (float)(IR.ir_6L() * 5.0) + (float)(IR.ir_5L() * 7.5) + (float)(IR.ir_4L() * 10.0) + (float)(IR.ir_3L() * 12.5) + (float)(IR.ir_2L() * 15.0) + (float)(IR.ir_1L() * 17.5);
    // readValue += (float)(IR.ir_9R() * -5.0) + (float)(IR.ir_10R() * -7.5) + (float)(IR.ir_11R() * -10.0) + (float)(IR.ir_12R() * -12.5) + (float)(IR.ir_13R() * -15.0) + (float)(IR.ir_14R() * -17.5);

    Serial.print("readValue: ");
    Serial.println(readValue);
    float sp = robot.PID_control(setpoint, readValue, lastTimePID, Kp, Ki, Kd);
    lastTimePID = millis();
    if (IR.ir_1L() == 1) // line vuông góc vs xe bên phải
        last_turn = 1;

    else if (IR.ir_14R() ==1)// line vuông góc vs xe bên trái
        last_turn = 2;
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
    
    // ý tưởng là tạo 1 biến lưu giá trị cũ của ir, nếu xe chạy ra khỏi line đen và all ir bắt line trắng thì xe sẽ quay về bên last_turn trả về
    if (IR.fullwhiteline() == 1) // nếu xe ra khỏi line đen -> all ir bắt line trắng thì sử dụng biến giá trị cũ điều khiển xe
    {
        if (last_turn == 1) 
        {
            lsp = speed - 50;
            rsp = -40;
        }
        else if (last_turn == 2)
        {
            lsp = -40;

            rsp = speed - 50;
        }
    }
    Serial.print("lsp: ");   Serial.println(lsp);
    Serial.print("rsp: ");  Serial.println(rsp);
    robot.motion(lsp, rsp);
    Serial.println("activing!");
    //}
}
void loop()
{
    //  IR.write_information(6); // in giĂ¡ trá»‹ so sĂ¡nh vĂ  giĂ¡ trá»‹ cá»§a 1 ir cá»¥ thá»ƒ
    // hĂ m tráº£ vá» giĂ¡ trá»‹ nhá»‹ phĂ¢n 0 - 1 tÆ°Æ¡ng á»©ng vs tá»«ng máº¯t IR Ä‘ang náº±m trĂªn line tráº¯ng hoáº·c Ä‘en

    // robot.motion(250, 250);
    followline();
}