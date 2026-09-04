// ============================================================
// tennis_field.ino - Main Tennis Field Tracer
// ============================================================

#include "motors.h"
#include "gyroscope.h"
#include "oled_display.h"
#include "tennis_drawing.h"

// ==================== SETUP ====================

void setup() {
  Serial.begin(9600);
  
  // Initialize components
  initMotors();
  initOLED();
  
  // Initialize and check gyroscope
  if (!initGyroscope()) {
    displayMessage("ERROR", "MPU6050 not found");
    while (1);
  }
  
  // Set drawing speed
  initDrawing(43.33);
  
  // Display ready message
  displayMessage("Tennis 1/20", "Starting in 3s");
  delay(3000);
  
  // Draw the tennis field
  drawTennisField();
  
  // Done
  displayMessage("COMPLETE!", "");
  while (1);
}

// ==================== LOOP ====================

void loop() {
  delay(1000);
}