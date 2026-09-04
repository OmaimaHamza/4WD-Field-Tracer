// ============================================================
// motors.cpp - Motor Control Implementation
// ============================================================

#include "motors.h"

// ==================== INITIALIZATION ====================

void initMotors() {
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_REV, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_REV, OUTPUT);
  stopMotors();
}

// ==================== CORE MOTOR FUNCTIONS ====================

void setMotors(int leftSpeed, int rightSpeed) {
  // Limit values
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
  // LEFT MOTOR
  if (leftSpeed >= 0) {
    digitalWrite(MOTOR_LEFT_FWD, HIGH);
    digitalWrite(MOTOR_LEFT_REV, LOW);
    analogWrite(MOTOR_LEFT_FWD, leftSpeed);
  } else {
    digitalWrite(MOTOR_LEFT_FWD, LOW);
    digitalWrite(MOTOR_LEFT_REV, HIGH);
    analogWrite(MOTOR_LEFT_REV, -leftSpeed);
  }
  
  // RIGHT MOTOR
  if (rightSpeed >= 0) {
    digitalWrite(MOTOR_RIGHT_FWD, HIGH);
    digitalWrite(MOTOR_RIGHT_REV, LOW);
    analogWrite(MOTOR_RIGHT_FWD, rightSpeed);
  } else {
    digitalWrite(MOTOR_RIGHT_FWD, LOW);
    digitalWrite(MOTOR_RIGHT_REV, HIGH);
    analogWrite(MOTOR_RIGHT_REV, -rightSpeed);
  }
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, 0);
}

// ==================== MOVEMENT FUNCTIONS ====================

void moveForward(unsigned long duration_ms) {
  setMotors(80, 80);
  delay(duration_ms);
  stopMotors();
}

void moveBackward(unsigned long duration_ms) {
  setMotors(-80, -80);
  delay(duration_ms);
  stopMotors();
}