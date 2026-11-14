#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
    private:
        uint8_t pin;
        uint currentSpeed;

    public:
        Motor(uint8_t motorPin) {
            pin = motorPin;
            currentSpeed = 0;
        }

        void attach(uint16_t freq, uint8_t resolution) {
            ledcAttach(pin, freq, resolution);
            write(0);
        }

        void write(uint speed) {
            currentSpeed = constrain(speed, 0, 255);
            ledcWrite(pin, currentSpeed);
        }

        int getSpeed() {
            return currentSpeed;
        }
};

#endif