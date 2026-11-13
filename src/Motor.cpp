#include "Motor.h"

class Motor {
    Motor Motor(uint8_t motorPin) {
    pin = motorPin;
    currentSpeed = 0;
}

void attach(uint32_t freq, uint8_t resolution) {
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
}
