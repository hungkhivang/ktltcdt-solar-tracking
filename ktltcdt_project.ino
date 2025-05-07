#include <LDRTracker.h>
// Khởi tạo: chân servo + chân LDR
LDRTracker tracker(2, 13, A0, A3, A1, A2, A5);

void setup() {
  tracker.begin();
  Serial.begin(9600);  // Bắt đầu giao tiếp Serial với Baud Rate 9600 (phải trùng với C++)
}

void loop() {
  tracker.update();
  float temp_val = tracker.getTempVal();
  int hor = tracker.getServoHori();
  int ver = tracker.getServoVert();

  // // Gửi dữ liệu theo định dạng: horizontal,vertical,temperature
  // Serial.print((float)hor);      // Gửi góc ngang (ép kiểu sang float)
  // Serial.print(",");             // Dấu phân cách
  // Serial.print((float)ver);      // Gửi góc đứng (ép kiểu sang float)
  // Serial.print(",");             // Dấu phân cách
  // Serial.println(temp_val, 2);   // Gửi nhiệt độ (float, 2 chữ số sau thập phân) và thêm ký tự xuống dòng


  Serial.print("TEMP:");
  Serial.print(temp_val, 2);
  Serial.print(",HORI:");
  Serial.print(hor);
  Serial.print(",VERT:");
  Serial.println(ver);  // Ensure println is used
}