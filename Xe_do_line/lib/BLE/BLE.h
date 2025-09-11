#ifndef BLE_H
#define BLE_H

class BLE {
   public: 
   static void setBLE(); // hàm setup cho kết nối bluetooth
  static void print_line_BT(); // đọc giá trị của ir trên line và gửi qua bluetooth
};
extern BLE BLE;
#endif