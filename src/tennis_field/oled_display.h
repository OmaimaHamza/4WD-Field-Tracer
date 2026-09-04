// ============================================================
// oled_display.h - OLED Display Library
// ============================================================

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>

// ==================== FUNCTION PROTOTYPES ====================

/**
 * Initialize the OLED display
 */
void initOLED();

/**
 * Display a message on the OLED screen
 * @param line1 First line text
 * @param line2 Second line text
 */
void displayMessage(const char* line1, const char* line2);

/**
 * Display the current gyroscope angle
 * @param angle Angle in degrees
 */
void displayAngle(float angle);

/**
 * Display the current position of the robot
 * @param x X coordinate in cm
 * @param y Y coordinate in cm
 */
void displayPosition(float x, float y);

#endif // OLED_DISPLAY_H