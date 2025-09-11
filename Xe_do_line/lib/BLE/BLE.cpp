#include <Arduino.h>
#include "BluetoothSerial.h"
#include "BLE.h"
#include "IR.h"
#include "motor.h"


BluetoothSerial BT;   // Khởi tạo Bluetooth

void BLE :: setBLE()
{
    BT.begin("ESP32_Robot");
}
void BLE :: print_line_BT() // in ra giá trị digital của từng IR khi ở trên line trắng(0) - đen(1)
{
  BT.print(IR.ir_1L()); BT.print(" | ");
  BT.print(IR.ir_2L()); BT.print(" | ");
  BT.print(IR.ir_3L()); BT.print(" | ");
  BT.print(IR.ir_4L()); BT.print(" | ");
  BT.print(IR.ir_5L()); BT.print(" | ");
  BT.print(IR.ir_6L()); BT.print(" | ");
  BT.print(IR.ir_7C()); BT.print(" | ");
  BT.print(IR.ir_8C()); BT.print(" | ");
  BT.print(IR.ir_15C()); BT.print(" | ");
  BT.print(IR.ir_16C()); BT.print(" | ");
  BT.print(IR.ir_9R()); BT.print(" | ");
  BT.print(IR.ir_10R()); BT.print(" | ");
  BT.print(IR.ir_11R()); BT.print(" | ");
  BT.print(IR.ir_12R()); BT.print(" | ");
  BT.print(IR.ir_13R()); BT.print(" | ");
  BT.println(IR.ir_14R()); BT.print(" | ");
}