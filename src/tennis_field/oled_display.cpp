// ============================================================
// oled_display.cpp - OLED Display Implementation
// ============================================================

#include "oled_display.h"

// ==================== GLOBAL OBJECT ====================

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ==================== INITIALIZATION ====================

void initOLED() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);
  
  u8g2.firstPage();
  do {
    u8g2.drawStr(10, 20, "Robot Traceur");
    u8g2.drawStr(10, 40, "Tennis 1/20");
  } while (u8g2.nextPage());
  delay(2000);
}

// ==================== DISPLAY FUNCTIONS ====================

void displayMessage(const char* line1, const char* line2) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 20, line1);
    u8g2.drawStr(5, 40, line2);
  } while (u8g2.nextPage());
  delay(500);
}

void displayAngle(float angle) {
  char buffer[20];
  dtostrf(angle, 6, 1, buffer);
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 12, "=== ROTATION ===");
    u8g2.setFont(u8g2_font_12x6LED_tf);
    u8g2.drawStr(5, 35, buffer);
    u8g2.drawStr(65, 35, "deg");
  } while (u8g2.nextPage());
}

void displayPosition(float x, float y) {
  char bufferX[10], bufferY[10];
  dtostrf(x, 5, 1, bufferX);
  dtostrf(y, 5, 1, bufferY);
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 12, "=== POSITION ===");
    u8g2.drawStr(5, 30, "X:");
    u8g2.drawStr(25, 30, bufferX);
    u8g2.drawStr(5, 50, "Y:");
    u8g2.drawStr(25, 50, bufferY);
  } while (u8g2.nextPage());
}