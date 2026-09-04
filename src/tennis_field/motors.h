// ============================================================
// motors.h - Motor Control Library
// ============================================================

#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

// ==================== PIN DEFINITIONS ====================
#define MOTOR_LEFT_FWD   5
#define MOTOR_LEFT_REV   6
#define MOTOR_RIGHT_FWD  10
#define MOTOR_RIGHT_REV  11

// ==================== FUNCTION PROTOTYPES ====================

/**
 * Initialize motor pins
 */
void initMotors();

/**
 * Set motor speeds
 * @param leftSpeed  -255 to 255 (negative = reverse)
 * @param rightSpeed -255 to 255 (negative = reverse)
 */
void setMotors(int leftSpeed, int rightSpeed);

/**
 * Stop both motors immediately
 */
void stopMotors();

/**
 * Move forward for a specified time
 * @param duration_ms Time in milliseconds
 */
void moveForward(unsigned long duration_ms);

/**
 * Move backward for a specified time
 * @param duration_ms Time in milliseconds
 */
void moveBackward(unsigned long duration_ms);

#endif // MOTORS_H