#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const int motorPins[] = {1, 2, 3, 4};

Adafruit_MPU6050 mpu;
WiFiUDP udp;
uint8_t packetBuffer[] = {0, 128, 128, 128}; // [0]=Throttle, [1]=Pitch, [2]=Roll, [3]=Yaw
uint8_t motorValues[] = {0, 0, 0, 0}; 

int clamp(float val);
void initMPU();
void initMotors();
void initWiFi();
void writeMotor(int motor, int value);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(500);
  Serial.println("Starting");


  initMPU();
  initMotors();
  initWiFi();

  Serial.println("System ready for flight");
}

float smoothedThrottle = 0.0;

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize > 0) {
      udp.read(packetBuffer, 4);
  }

  float userThrottle = packetBuffer[0];
  float userPitch    = (packetBuffer[1] - 128) / 12.8; // add or sub 10 to the motor value
  float userRoll     = (packetBuffer[2] - 128) / 12.8;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float pitchCorrection = (g.gyro.y * 10.0) + userPitch;
  float rollCorrection  = (g.gyro.x * 10.0) + userRoll;

  if (userThrottle < 10) {
    smoothedThrottle = 0.0;
    for (int i = 0; i < 4; i++) {
      motorValues[i] = 0;
      writeMotor(i, 0);
    }
  } 
  else {
    smoothedThrottle += (userThrottle - smoothedThrottle) * 0.15;

    motorValues[0] = clamp(smoothedThrottle - pitchCorrection + rollCorrection); // Front Left
    motorValues[1] = clamp(smoothedThrottle - pitchCorrection - rollCorrection); // Front Right
    motorValues[2] = clamp(smoothedThrottle + pitchCorrection + rollCorrection); // Back Left
    motorValues[3] = clamp(smoothedThrottle + pitchCorrection - rollCorrection); // Back Right

    for (int i = 0; i < 4; i++) {
      writeMotor(i, motorValues[i]);
    }
  }
}

void initMPU() {
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
}

void initMotors() {
  const int freq = 20000;
  const int resolution = 8;
  for (int i = 0; i < 4; i++) {
    ledcAttach(motorPins[i], freq, resolution);
  }
}

void writeMotor(int motor, int value) {
  ledcWrite(motorPins[motor], value);
}

void initWiFi() {
  WiFi.softAP("Tal's Drone", "");
  udp.begin(5050);
}

int clamp(float val) {
  if (val > 255) return 255;
  if (val < 25) return 0;
  return (int)val;
}
