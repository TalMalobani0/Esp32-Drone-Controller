#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
private:
    uint8_t pin;
    int currentSpeed;

public:
    Motor(uint8_t motorPin);
    void attach(uint, uint16_t freq = 20000, uint8_t resolution = 8);
    void write(int speed);
    int getSpeed();
};

#endif