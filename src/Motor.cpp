#include "Motor.h"

Motor::Motor(uint8_t motorPin) {
    pin = motorPin;
    currentSpeed = 0;
}

void Motor::attach(uint32_t freq, uint8_t resolution) {
    ledcAttach(pin, freq, resolution);
    write(0);
}

void Motor::write(int speed) {
    currentSpeed = constrain(speed, 0, 255);
    ledcWrite(pin, currentSpeed);
}

int Motor::getSpeed() {
    return currentSpeed;
}