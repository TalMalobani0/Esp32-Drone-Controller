#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
    private:
        uint8_t pin;
        uint16_t currentSpeed;

    public:
        Motor(uint8_t motorPin) {
            pin = motorPin;
            currentSpeed = 0;
        }

        void attach(uint32_t freq = 20000, uint8_t resolution = 8) {
            ledcAttach(pin, freq, resolution);
            write(0);
        }

        void write(int speed) {
            currentSpeed = constrain(speed, 0, 255);
            ledcWrite(pin, currentSpeed);
        }

        int getSpeed() {
            return currentSpeed;
        }
};

#endif