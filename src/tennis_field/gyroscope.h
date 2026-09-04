// ============================================================
// gyroscope.h - MPU6050 Gyroscope Library
// ============================================================

#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>

// ==================== FUNCTION PROTOTYPES ====================

/**
 * Initialize the MPU6050 gyroscope
 * @return true if sensor found and initialized
 */
bool initGyroscope();

/**
 * Calibrate the gyroscope (keep robot stationary)
 * Updates the gyro offset value
 */
void calibrateGyroscope();

/**
 * Get current rotation angle in degrees
 * @return Angle in degrees (can be positive or negative)
 */
float getGyroAngle();

/**
 * Reset the gyroscope angle to 0
 */
void resetGyroAngle();

/**
 * Turn the robot 90° to the right
 * Uses gyroscope for precise angle measurement
 */
void turnRight();

/**
 * Turn the robot 90° to the left
 * Uses gyroscope for precise angle measurement
 */
void turnLeft();

/**
 * Set rotation calibration value
 * @param calibration Offset in degrees (-10 to +10)
 */
void setRotationCalibration(int calibration);

#endif // GYROSCOPE_H