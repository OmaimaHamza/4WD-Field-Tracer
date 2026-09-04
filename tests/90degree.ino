// ============================================================
// 90° Rotation Using MPU6050 + OLED (Angle Display)
// ============================================================

#include <Wire.h>
#include <MPU6050.h>
#include <U8g2lib.h>

// ==================== OLED ====================
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ==================== MPU6050 ====================
MPU6050 mpu;

// Motor Pins
#define MOTOR_LEFT_FWD   5
#define MOTOR_LEFT_REV   6
#define MOTOR_RIGHT_FWD  10
#define MOTOR_RIGHT_REV  11

// Motor Power Levels
const int MOTOR_POWER = 80;
const int TURN_POWER_FAST = 100;
const int TURN_POWER_SLOW = 60;

// Calibration
const int CALIBRATION = 0;

// gyroscope variables
float gyroOffset = 0;
float robotAngle = 0;
unsigned long lastTime = 0;

// ==================== OLED Funtions ====================

void initOLED() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);
  
  u8g2.firstPage();
  do {
    u8g2.drawStr(10, 20, "90 degree Rotation");
    u8g2.drawStr(10, 40, "Ready...");
  } while (u8g2.nextPage());
  delay(2000);
}

void afficherAngle(float angle) {
  char buffer[20];
  dtostrf(angle, 6, 1, buffer); //float to text
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_12x6LED_tf);
    u8g2.drawStr(5, 35, "Angle:");
    u8g2.drawStr(55, 35, buffer);
    u8g2.drawStr(105, 35, "deg");
    
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 55, "Wait...");
  } while (u8g2.nextPage());
}

void afficherAngleFinal(float angle) {
  char buffer[20];
  dtostrf(angle, 6, 1, buffer);
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 12, "=== ROTATION COMPLETED ===");
    
    u8g2.setFont(u8g2_font_12x6LED_tf);
    u8g2.drawStr(5, 40, buffer);
    u8g2.drawStr(65, 40, "deg");
  } while (u8g2.nextPage());
}

// ==================== GYROSCOPE Functions ====================

void calibrateGyro() {
  u8g2.firstPage();
  do {
    u8g2.drawStr(10, 20, "Calibration...");
    u8g2.drawStr(10, 40, "Do Not Move It !");
  } while (u8g2.nextPage());
  
  float sum = 0;
  for (int i = 0; i < 200; i++) {
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    sum += gz;
    delay(3);
  }
  
  gyroOffset = sum / 200;
  lastTime = micros();
  robotAngle = 0;
  
  u8g2.firstPage();
  do {
    u8g2.drawStr(10, 30, "Calibration OK !");
  } while (u8g2.nextPage());
  delay(1000);
}

float getGyroAngle() {
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  
  float gyroZ = (gz - gyroOffset) / 131.0;
  
  unsigned long now = micros();
  float dt = (now - lastTime) / 1000000.0;
  lastTime = now;
  
  robotAngle += gyroZ * dt;
  return robotAngle;
}

void resetGyroAngle() {
  robotAngle = 0;
  lastTime = micros();
}

// ==================== MOTORS FUNCTIONS ====================

void setMotors(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
  if (leftSpeed >= 0) {
    digitalWrite(MOTOR_LEFT_FWD, HIGH);
    digitalWrite(MOTOR_LEFT_REV, LOW);
    analogWrite(MOTOR_LEFT_FWD, leftSpeed);
  } else {
    digitalWrite(MOTOR_LEFT_FWD, LOW);
    digitalWrite(MOTOR_LEFT_REV, HIGH);
    analogWrite(MOTOR_LEFT_REV, -leftSpeed);
  }
  
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

// ==================== ROTATION 90° ====================

void tourner90Droite() {
  float targetAngle = 90.0 + CALIBRATION;
  
  resetGyroAngle();
  delay(50);
  
  // Afficher l'angle pendant la rotation
  afficherAngle(0);
  
  // Phase rapide
  setMotors(TURN_POWER_FAST, -TURN_POWER_FAST);
  while (abs(getGyroAngle()) < targetAngle - 5) {
    afficherAngle(getGyroAngle());
    delay(5);
  }
  
  // Phase lente
  setMotors(TURN_POWER_SLOW, -TURN_POWER_SLOW);
  while (abs(getGyroAngle()) < targetAngle) {
    afficherAngle(getGyroAngle());
    delay(5);
  }
  
  stopMotors();
  
  // Afficher l'angle final
  afficherAngleFinal(getGyroAngle());
  delay(3000);
}

// ==================== SETUP ====================

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Initialiser OLED
  initOLED();
  
  // Initialiser MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    u8g2.firstPage();
    do {
      u8g2.drawStr(10, 30, "MPU6050 ERROR");
    } while (u8g2.nextPage());
    while (1);
  }
  
  // Configuration moteurs
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_REV, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_REV, OUTPUT);
  
  // Calibration
  calibrateGyro();
  
  // Attente avant rotation
  u8g2.firstPage();
  do {
    u8g2.drawStr(10, 20, "90° Rotation");
    u8g2.drawStr(10, 40, "In 3 seconds...");
  } while (u8g2.nextPage());
  delay(3000);
  
  // Exécuter la rotation
  tourner90Droite();
}

void loop() {
  delay(1000);
}