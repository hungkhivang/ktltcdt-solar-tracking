#include "LDRTracker.h"

LDRTracker::LDRTracker(int hPin, int vPin,
    int ldrTL, int ldrTR, int ldrBL, int ldrBR, int tempSensor,
    int hLow, int hHigh,
    int vLow, int vHigh,
    int tolerance, int delayTime)
    : ldrTL(ldrTL), ldrTR(ldrTR), ldrBL(ldrBL), ldrBR(ldrBR),
    servohori(180), servovert(45),
    servohoriLimitLow(hLow), servohoriLimitHigh(hHigh),
    servovertLimitLow(vLow), servovertLimitHigh(vHigh),
    tolerance(tolerance), dtime(delayTime),
    tempSensor(tempSensor) // <-- Lưu tempSensor vào biến thành viên
{
    horizontal.attach(hPin);
    vertical.attach(vPin);
}
//đọc trạng thái ban đầu khi bắt đầu chạy
void LDRTracker::begin() {
    horizontal.write(servohori);
    vertical.write(servovert);
    delay(2500);
}
//
void LDRTracker::update() {
    //đọc dữ liêu trả về từ cảm biến ánh sáng
    int lt = analogRead(ldrTL);
    int rt = analogRead(ldrTR);
    int ld = analogRead(ldrBL);
    int rd = analogRead(ldrBR);
    //chia trung bình giá trị giữa các cặp cảm biến ánh sáng
    int avt = (lt + rt) / 2;
    int avd = (ld + rd) / 2;
    int avl = (lt + ld) / 2;
    int avr = (rt + rd) / 2;
    //Tính độ sai lệch giữa cảm biến trên & dưới, và trái & phải
    int dvert = avt - avd;
    int dhoriz = avl - avr;
    //Đọc dữ liệu từ cảm biến nhiệt độ
    int temp_adc_val = analogRead(tempSensor);
    float temp_val = (temp_adc_val * 4.88)/ 10; //Đổi tính hiệu analog ra độ C

    //điều kiện thay đổi servo theo chiều vertical
    if (abs(dvert) > tolerance) {
        if (avt > avd) {
            servovert++;
            delay(500);
            if (servovert > servovertLimitHigh) servovert = servovertLimitHigh;
        }
        else {
            servovert--;
            delay(500);
            if (servovert < servovertLimitLow) servovert = servovertLimitLow;
        }
        vertical.write(servovert);
    }
    //điều kiện thay đổi servo theo horizon
    if (abs(dhoriz) > tolerance) {
        if (avl > avr) {
            servohori--;
            delay(500);
            if (servohori < servohoriLimitLow) servohori = servohoriLimitLow;
        }
        else {
            servohori++;
            delay(500);
            if (servohori > servohoriLimitHigh) servohori = servohoriLimitHigh;
        }
        horizontal.write(servohori);
    }

    delay(dtime);
}
int LDRTracker::getServoHori() const {
    return servohori;
}

int LDRTracker::getServoVert() const {
    return servovert;
}
float LDRTracker::getTempVal() const {
    int temp_adc_val = analogRead(tempSensor);
    return (temp_adc_val * 4.88) / 10.0;
}


