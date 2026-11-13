#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
private:
    uint8_t pin;
    int currentSpeed;

public:
    Motor(uint8_t motorPin) {
        pin = motorPin;
        currentSpeed = 0;
    }

    // Attach method
    void attach(uint32_t freq = 20000, uint8_t resolution = 8) {
        ledcAttach(pin, freq, resolution);
        write(0); // Ensure it starts off
    }

    // Write method
    void write(int speed) {
        currentSpeed = constrain(speed, 0, 255);
        ledcWrite(pin, currentSpeed);
    }

    // Get speed method
    int getSpeed() {
        return currentSpeed;
    }
};

#endif