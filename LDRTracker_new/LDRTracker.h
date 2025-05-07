#ifndef LDRTRACKER_H
#define LDRTRACKER_H

#include <Arduino.h>
#include <Servo.h>

class LDRTracker {
public:
    LDRTracker(int hPin, int vPin,
        int ldrTL, int ldrTR, int ldrBL, int ldrBR,int tempSensor,
        //giới hạn xoay của servo
        int hLow = 5, int hHigh = 175,
        int vLow = 1, int vHigh = 100,
        //độ sai lệch cho phép giữa các cảm biến biến ánh sáng
        int tolerance = 90, 
        int delayTime = 2000);

    int getServoHori() const;
    int getServoVert() const;
    float getTempVal() const;


    void begin();
    void update();

private:
    Servo horizontal;
    Servo vertical;

    int servohori;
    int servovert;

    int servohoriLimitHigh;
    int servohoriLimitLow;
    int servovertLimitHigh;
    int servovertLimitLow;

    int ldrTL, ldrTR, ldrBL, ldrBR, tempSensor;
    int tolerance;
    int dtime;
};

#endif
#pragma once
