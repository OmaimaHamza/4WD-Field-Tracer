// ============================================================
// gyroscope.cpp - MPU6050 Gyroscope Implementation
// ============================================================

#include "gyroscope.h"
#include "motors.h"

// ==================== GLOBAL VARIABLES ====================

MPU6050 mpu;

float gyroOffset = 0.0;
float robotAngle = 0.0;
unsigned long lastTime = 0;

// Rotation parameters
const int TURN_POWER_FAST = 100;
const int TURN_POWER_SLOW = 60;

int rotationCalibration = -7;  // Adjust for 90° precision

// ==================== INITIALIZATION ====================

bool initGyroscope() {
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 not found!");
    return false;
  }
  
  Serial.println("MPU6050 connected!");
  calibrateGyroscope();
  return true;
}

// ==================== CALIBRATION ====================

void calibrateGyroscope() {
  Serial.println("Calibrating gyroscope... Do not move!");
  
  float sum = 0.0;
  for (int i = 0; i < 200; i++) {
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    sum += gz;
    delay(3);
  }
  
  gyroOffset = sum / 200.0;
  lastTime = micros();
  robotAngle = 0.0;
  
  Serial.println("Calibration complete!");
}

// ==================== ANGLE MEASUREMENT ====================

float getGyroAngle() {
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  
  // Convert raw value to degrees per second
  float gyroZ = (gz - gyroOffset) / 131.0;
  
  // Calculate time elapsed
  unsigned long now = micros();
  float dt = (now - lastTime) / 1000000.0;
  lastTime = now;
  
  // Integrate to get angle
  robotAngle += gyroZ * dt;
  return robotAngle;
}

void resetGyroAngle() {
  robotAngle = 0.0;
  lastTime = micros();
}

// ==================== CALIBRATION FUNCTIONS ====================

void setRotationCalibration(int calibration) {
  rotationCalibration = calibration;
}

// ==================== ROTATION FUNCTIONS ====================

void turnRight() {
  // Quick recalibration before turning
  float sum = 0;
  for (int i = 0; i < 50; i++) {
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    sum += gz;
    delay(2);
  }
  gyroOffset = (gyroOffset + sum / 50) / 2;
  
  float target = 94.0 + rotationCalibration;
  resetGyroAngle();
  delay(50);
  
  // Fast phase
  setMotors(TURN_POWER_FAST, -TURN_POWER_FAST);
  while (abs(getGyroAngle()) < target - 5) {
    delay(5);
  }
  
  // Slow phase for precision
  setMotors(TURN_POWER_SLOW, -TURN_POWER_SLOW);
  while (abs(getGyroAngle()) < target) {
    delay(5);
  }
  
  stopMotors();
  delay(500);
}

void turnLeft() {
  // Quick recalibration before turning
  float sum = 0;
  for (int i = 0; i < 50; i++) {
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    sum += gz;
    delay(2);
  }
  gyroOffset = (gyroOffset + sum / 50) / 2;
  
  float target = 94.0 + rotationCalibration;
  resetGyroAngle();
  delay(50);
  
  // Fast phase
  setMotors(-TURN_POWER_FAST, TURN_POWER_FAST);
  while (abs(getGyroAngle()) < target - 5) {
    delay(5);
  }
  
  // Slow phase for precision
  setMotors(-TURN_POWER_SLOW, TURN_POWER_SLOW);
  while (abs(getGyroAngle()) < target) {
    delay(5);
  }
  
  stopMotors();
  delay(500);
}