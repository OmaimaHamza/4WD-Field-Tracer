// ============================================================
// ROBOT TRACEUR - RECTANGLE + LIGNE CENTRALE + CERCLE
// Échelle 1/80
// ============================================================

#include <Wire.h>
#include <MPU6050.h>
#include <U8g2lib.h>

// ==================== OLED ====================
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// ==================== MPU6050 ====================
MPU6050 mpu;

// Pins moteurs
#define MOTOR_LEFT_FWD   5
#define MOTOR_LEFT_REV   6
#define MOTOR_RIGHT_FWD  10
#define MOTOR_RIGHT_REV  11

// ==================== VITESSE (POWER=80, 130cm/3s) ====================
const float VITESSE_CM_S = 43.33;
const int MOTOR_POWER = 80;
const int TURN_POWER_FAST = 100;
const int TURN_POWER_SLOW = 60;

// Calibrations
const int CALIBRATION_DROITE = -5;
const int CALIBRATION_ROTATION = -7;

// ==================== DIMENSIONS 1/80 (cm) ====================
const float LONGUEUR = 150.0;
const float LARGEUR = 112.5;
const float DEMI_LONGUEUR = 75.0;
const float DEMI_LARGEUR = 56.25;

// ==================== TEMPS CALCULÉS (ms) ====================
const unsigned long T_LONGUEUR = 3462;
const unsigned long T_LARGEUR = 2596;
const unsigned long T_DEMI_LONGUEUR = 1731;
const unsigned long T_DEMI_LARGEUR = 1298;

// ==================== VARIABLES ====================
float gyroOffset = 0;
float robotAngle = 0;
unsigned long lastTime = 0;

// ==================== SETUP ====================
void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_REV, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_REV, OUTPUT);
  
  initOLED();
  calibrateGyro();
  
  afficherMessage("Trace 1/80", "Debute dans 3s");
  delay(3000);
  
  tracerTerrain();
  
  afficherMessage("Termine!", "");
  while(1);
}

// ==================== PLAN DE TRACÉ ====================
void tracerTerrain() {
  
  // === 1. PREMIÈRE DEMI LONGUEUR ===
  afficherMessage("1. Demi longueur", "75 cm");
  avancer(T_DEMI_LONGUEUR);
  tourner90Droite();
  
  // === 2. PREMIÈRE LARGEUR ===
  afficherMessage("2. Largeur", "112.5 cm");
  avancer(T_LARGEUR);
  tourner90Droite();
  
  // === 3. DEUXIÈME LONGUEUR ===
  afficherMessage("3. Longueur", "150 cm");
  avancer(T_LONGUEUR);
  tourner90Droite();
  
  // === 4. DEUXIÈME LARGEUR ===
  afficherMessage("4. Largeur", "112.5 cm");
  avancer(T_LARGEUR);
  tourner90Droite();
  
  // === 5. DEUXIÈME DEMI LONGUEUR (retour au point de départ) ===
  afficherMessage("5. Demi longueur", "75 cm");
  avancer(T_DEMI_LONGUEUR);
  tourner90Droite();
  
  // === 6. LIGNE CENTRALE ===
  afficherMessage("6. Ligne centrale", "");
  
  // Aller au centre
  avancer(T_DEMI_LONGUEUR);
  tourner90Droite();
  
  // Tracer la ligne médiane
  afficherMessage("Trace ligne", "centrale");
  avancer(T_LARGEUR);
  
  // Revenir au centre
  reculer(T_DEMI_LARGEUR);
  tournerGauche();
  
  // === 7. CERCLE CENTRAL ===
  afficherMessage("7. Cercle central", "360°");
  tracerCercleCentral();
  
  afficherMessage("FIN", "");
}

// ==================== FONCTIONS BASE ====================
void avancer(unsigned long temps_ms) {
  int left = MOTOR_POWER;
  int right = MOTOR_POWER;
  
  if (CALIBRATION_DROITE > 0) {
    right = MOTOR_POWER - CALIBRATION_DROITE;
  } else if (CALIBRATION_DROITE < 0) {
    left = MOTOR_POWER + CALIBRATION_DROITE;
  }
  
  setMotors(left, right);
  delay(temps_ms);
  stopMotors();
  delay(200);
}

void reculer(unsigned long temps_ms) {
  setMotors(-MOTOR_POWER, -MOTOR_POWER);
  delay(temps_ms);
  stopMotors();
  delay(200);
}

void setMotors(int left, int right) {
  left = constrain(left, -255, 255);
  right = constrain(right, -255, 255);
  
  // Moteur gauche
  if (left >= 0) {
    digitalWrite(MOTOR_LEFT_FWD, HIGH);
    digitalWrite(MOTOR_LEFT_REV, LOW);
    analogWrite(MOTOR_LEFT_FWD, left);
  } else {
    digitalWrite(MOTOR_LEFT_FWD, LOW);
    digitalWrite(MOTOR_LEFT_REV, HIGH);
    analogWrite(MOTOR_LEFT_REV, -left);
  }
  
  // Moteur droit
  if (right >= 0) {
    digitalWrite(MOTOR_RIGHT_FWD, HIGH);
    digitalWrite(MOTOR_RIGHT_REV, LOW);
    analogWrite(MOTOR_RIGHT_FWD, right);
  } else {
    digitalWrite(MOTOR_RIGHT_FWD, LOW);
    digitalWrite(MOTOR_RIGHT_REV, HIGH);
    analogWrite(MOTOR_RIGHT_REV, -right);
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

// ==================== GYROSCOPE ====================
void calibrateGyro() {
  afficherMessage("Calibration...", "Ne pas bouger !");
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
  afficherMessage("Calibration OK", "");
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

void tourner90Droite() {
  float target = 90.0 + CALIBRATION_ROTATION;
  resetGyroAngle();
  delay(50);
  
  setMotors(TURN_POWER_FAST, -TURN_POWER_FAST);
  while (abs(getGyroAngle()) < target - 5) {
    delay(5);
  }
  
  setMotors(TURN_POWER_SLOW, -TURN_POWER_SLOW);
  while (abs(getGyroAngle()) < target) {
    delay(5);
  }
  
  stopMotors();
  robotAngle += 90;
  delay(500);
}

void tournerGauche() {
  float target = 90.0 + CALIBRATION_ROTATION;
  resetGyroAngle();
  delay(50);
  
  setMotors(-TURN_POWER_FAST, TURN_POWER_FAST);
  while (abs(getGyroAngle()) < target - 5) {
    delay(5);
  }
  
  setMotors(-TURN_POWER_SLOW, TURN_POWER_SLOW);
  while (abs(getGyroAngle()) < target) {
    delay(5);
  }
  
  stopMotors();
  robotAngle -= 90;
  delay(500);
}

void tracerCercleCentral() {
  resetGyroAngle();
  delay(100);
  
  setMotors(TURN_POWER_FAST, -TURN_POWER_FAST);
  
  while (abs(getGyroAngle()) < 360) {
    delay(5);
  }
  
  stopMotors();
  delay(500);
}

// ==================== OLED ====================
void initOLED() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.firstPage();
  do {
    u8g2.drawStr(10, 20, "Robot Traceur");
    u8g2.drawStr(10, 40, "Echelle 1/80");
  } while (u8g2.nextPage());
  delay(2000);
}

void afficherMessage(const char* ligne1, const char* ligne2) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(5, 20, ligne1);
    u8g2.drawStr(5, 40, ligne2);
  } while (u8g2.nextPage());
  delay(500);
}

void loop() {
  delay(1000);
}