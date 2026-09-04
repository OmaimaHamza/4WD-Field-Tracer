// ============================================================
// tennis_drawing.cpp - Tennis Field Drawing Implementation
// ============================================================

#include "tennis_drawing.h"
#include "motors.h"
#include "gyroscope.h"
#include "oled_display.h"

// ==================== CONSTANTS ====================

float SPEED_CM_S = 43.33;  // Default speed

// Field dimensions (1/20 scale in cm)
const float COULOIR = 20.0;
const float LONGUEUR_TOTALE = 118.0;
const float LARGEUR_SIMPLE = 41.0;
const float DISTANCE_AU_FOND = 27.0;
const float DISTANCE_AU_FILET = 32.0;

// ==================== CALIBRATION ====================

const int CALIBRATION_DROITE = 5;  // Adjust for straight line

// ==================== INITIALIZATION ====================

void initDrawing(float speed_cm_s) {
  SPEED_CM_S = speed_cm_s;
}

// ==================== UTILITY FUNCTIONS ====================

unsigned long distanceToTime(float distance_cm) {
  return (unsigned long)((distance_cm / SPEED_CM_S) * 1000);
}

// ==================== MOVEMENT FUNCTIONS ====================

void drawLine(float distance_cm) {
  unsigned long time_ms = distanceToTime(distance_cm);
  
  int leftPower = 80;
  int rightPower = 80;
  
  // Apply straight line calibration
  if (CALIBRATION_DROITE > 0) {
    rightPower = 80 - CALIBRATION_DROITE;
  } else if (CALIBRATION_DROITE < 0) {
    leftPower = 80 + CALIBRATION_DROITE;
  }
  
  leftPower = constrain(leftPower, 60, 255);
  rightPower = constrain(rightPower, 60, 255);
  
  setMotors(leftPower, rightPower);
  delay(time_ms);
  stopMotors();
}

void moveWithoutDrawing(float distance_cm) {
  unsigned long time_ms = distanceToTime(distance_cm);
  
  int leftPower = 80;
  int rightPower = 80;
  
  if (CALIBRATION_DROITE > 0) {
    rightPower = 80 - CALIBRATION_DROITE;
  } else if (CALIBRATION_DROITE < 0) {
    leftPower = 80 + CALIBRATION_DROITE;
  }
  
  leftPower = constrain(leftPower, 60, 255);
  rightPower = constrain(rightPower, 60, 255);
  
  setMotors(leftPower, rightPower);
  delay(time_ms);
  stopMotors();
}

// ==================== DRAWING THE FIELD ====================

void drawTennisField() {
  displayMessage("Tennis 1/20", "Starting...");
  delay(1000);
  
  // === 1 ===
  displayMessage("1", "6 cm");
  drawLine(COULOIR);
  turnRight();
  
  // === 2 ===
  displayMessage("2", "118 cm");
  drawLine(LONGUEUR_TOTALE);
  turnRight();
  
  // === 3 ===
  displayMessage("3", "6 cm");
  drawLine(COULOIR);
  turnRight();
  
  // === 4 ===
  displayMessage("4", "118 cm");
  drawLine(LONGUEUR_TOTALE);
  turnLeft();
  
  // === 5 ===
  displayMessage("5", "47 cm");
  drawLine(LARGEUR_SIMPLE + COULOIR);
  turnLeft();
  
  // === 6 ===
  displayMessage("6", "118 cm");
  drawLine(LONGUEUR_TOTALE);
  turnLeft();
  
  // === 7 ===
  displayMessage("7", "6 cm");
  drawLine(COULOIR);
  turnLeft();
  
  // === 8 ===
  displayMessage("8", "118 cm");
  drawLine(LONGUEUR_TOTALE);
  turnRight();
  
  // === 8-1 ===
  displayMessage("8-1", "41 cm");
  drawLine(LARGEUR_SIMPLE);
  turnRight();
  
  // === 8-2 ===
  displayMessage("8-2", "27 cm");
  drawLine(DISTANCE_AU_FOND);
  turnRight();
  
  // === 11 ===
  displayMessage("11", "41 cm");
  drawLine(LARGEUR_SIMPLE);
  turnLeft();
  
  // === 11-1 ===
  displayMessage("11-1", "64 cm");
  drawLine(DISTANCE_AU_FILET * 2);
  turnLeft();
  
  // === 10 ===
  displayMessage("10", "41 cm");
  drawLine(LARGEUR_SIMPLE);
  
  // === 10-1 ===
  displayMessage("10-1", "reculer 20 cm");
  moveBackward(distanceToTime(20));
  turnLeft();
  
  // === 12 ===
  displayMessage("12", "64 cm");
  drawLine(DISTANCE_AU_FILET * 2);
  turnRight();
  
  // === 12-1 ===
  displayMessage("12-1", "20 cm");
  drawLine(20);
  turnRight();
  
  // === 12-2 ===
  displayMessage("12-2", "91 cm");
  drawLine(91);
  turnRight();
  
  // === 9 ===
  displayMessage("9", "41 cm");
  drawLine(LARGEUR_SIMPLE);
  
  displayMessage("DONE!", "");
}