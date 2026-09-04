// ============================================================
// tennis_drawing.h - Tennis Field Drawing Library
// ============================================================

#ifndef TENNIS_DRAWING_H
#define TENNIS_DRAWING_H

#include <Arduino.h>

// ==================== FUNCTION PROTOTYPES ====================

/**
 * Initialize drawing parameters
 * @param speed_cm_s Robot speed in cm/s
 */
void initDrawing(float speed_cm_s);

/**
 * Convert distance to time in milliseconds
 * @param distance_cm Distance in centimeters
 * @return Time in milliseconds
 */
unsigned long distanceToTime(float distance_cm);

/**
 * Draw the complete tennis field
 */
void drawTennisField();

/**
 * Draw a single line segment
 * @param distance_cm Length of line in centimeters
 */
void drawLine(float distance_cm);

/**
 * Move without drawing
 * @param distance_cm Length to move in centimeters
 */
void moveWithoutDrawing(float distance_cm);

#endif // TENNIS_DRAWING_H