#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const int motorPins[] = {1, 2, 3, 4}; 

Adafruit_MPU6050 mpu;
WiFiUDP udp;
uint8_t packetBuffer[] = {0, 0, 0, 0}; // [0]=Throttle, [1]=Pitch, [2]=Roll, [3]=Yaw
uint8_t motorValues[] = {0, 0, 0, 0}; 

int clamp(float val);
void initMPU();
void initMotors();
void initWiFi();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(500);

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

  float stabilityRoll  = -a.acceleration.y * 10.0; 
  float stabilityPitch = a.acceleration.x * 10.0;

  float rollTotal  = stabilityRoll + userRoll;
  float pitchTotal = stabilityPitch + userPitch;

  #pragma region write motor values
  if (userThrottle < 10) {
    smoothedThrottle = 0.0;
    for (int i = 0; i < 4; i++) {
      motorValues[i] = 0;
      ledcWrite(motorPins[i], 0);
    }
  } 
  else {
    smoothedThrottle += (userThrottle - smoothedThrottle) * 0.15;

    motorValues[0] = clamp(smoothedThrottle - pitchTotal + rollTotal); // Front Left
    motorValues[1] = clamp(smoothedThrottle - pitchTotal - rollTotal); // Front Right
    motorValues[2] = clamp(smoothedThrottle + pitchTotal + rollTotal); // Back Left
    motorValues[3] = clamp(smoothedThrottle + pitchTotal - rollTotal); // Back Right

    for (int i = 0; i < 4; i++) {
      ledcWrite(motorPins[i], motorValues[i]);
    }
  }
  #pragma endregion

  #pragma Print to serial
  // static unsigned long lastPrint = 0;
  // if (millis() - lastPrint > 100) { 
  //     Serial.print("Raw throttle: ");
  //     Serial.print(packetBuffer[0]);
  //     Serial.print(" | Smoothed throttle: ");
  //     Serial.print(smoothedThrottle, 1);
  //     Serial.print(" | M1: ");
  //     Serial.print(motorValues[0]);
  //     Serial.print(" M2: ");
  //     Serial.print(motorValues[1]);
  //     Serial.print(" M3: ");
  //     Serial.print(motorValues[2]);
  //     Serial.print(" M4: ");
  //     Serial.println(motorValues[3]);

  //     lastPrint = millis();
  // }
  #pragma endregion
}

#pragma region functions
void initMPU() {
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
}

void initMotors() {
  const int freq = 20000;
  const int resolution = 8
  for (int i = 0; i < 4; i++) {
    ledcAttach(motorPins[i], freq, resolution); 
  }
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
#pragma endregion