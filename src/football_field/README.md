 
# Football Field Tracer (Scale 1/80)  
<div align="center"> <img width="800" height="400" alt="WhatsApp Image 2026-10-21 at 8 23 32 PM" src="https://github.com/user-attachments/assets/fa0efbd9-88d8-4d65-9019-fec85c16593d" />
</div>  

## Overview

This document describes the football field tracing module of the 4WD Field Tracer Robot. The robot autonomously draws a football field outline, center line, and center circle at 1/80 scale using a gyroscope-based navigation system.

---

## Hardware Requirements

| Component | Specification | Quantity |
|-----------|--------------|----------|
| Microcontroller | Arduino Uno R3 | 1 |
| Motor Driver | L298N H-Bridge | 1 |
| DC Motors | 6V with wheels | 4 |
| Gyroscope | MPU6050 (6-axis IMU) | 1 |
| OLED Display | SSD1306 128x64 I2C | 1 |
| Battery | 2x 18650 Li-ion (7.4V) | 1 |
| Chassis | 4WD Robot Platform | 1 |

### Pin Configuration

| Component | Arduino Pin |
|-----------|-------------|
| Left Motor Forward | 5 |
| Left Motor Reverse | 6 |
| Right Motor Forward | 10 |
| Right Motor Reverse | 11 |
| MPU6050 SDA | A4 |
| MPU6050 SCL | A5 |
| OLED SDA | A4 |
| OLED SCL | A5 |

### Wiring Notes

- The L298N ENA and ENB jumpers must be installed for full motor power.
- Both MPU6050 and OLED share the same I2C bus (A4/A5).
- The L298N requires a common ground with the Arduino.
- The battery connects to the L298N +12V and GND terminals.

---

## Required Libraries

| Library | Version | Purpose |
|---------|---------|---------|
| Wire | Built-in | I2C communication |
| MPU6050 | Electronic Cats | Gyroscope reading |
| U8g2 | Oliver Kraus | OLED display control |

Install via Arduino IDE Library Manager.

---

## Field Dimensions (1/80 Scale)

| Element | Real Size (m) | Scale Size (cm) |
|---------|--------------|-----------------|
| Field Length | 120 | 150.0 |
| Field Width | 90 | 112.5 |
| Half Length | - | 75.0 |
| Half Width | - | 56.25 |

### Calculated Movement Times

Based on a measured speed of 43.33 cm/s:

| Movement | Distance (cm) | Time (ms) |
|----------|--------------|-----------|
| Full Length | 150.0 | 3462 |
| Full Width | 112.5 | 2596 |
| Half Length | 75.0 | 1731 |
| Half Width | 56.25 | 1298 |

---

## Code Architecture

### File Structure

```
1-80FIELD.ino
```

This is a single-file Arduino sketch containing all functions.

### Function Reference

| Function | Purpose |
|----------|---------|
| setup() | Initializes serial, I2C, MPU6050, motors, OLED, and gyroscope calibration |
| tracerTerrain() | Main drawing sequence controller |
| avancer(ms) | Moves forward for a specified duration |
| reculer(ms) | Moves backward for a specified duration |
| setMotors(left, right) | Sets individual motor speeds with direction control |
| stopMotors() | Stops all motors immediately |
| calibrateGyro() | Measures and stores gyroscope offset at startup |
| getGyroAngle() | Returns the current integrated angle in degrees |
| resetGyroAngle() | Resets the angle counter to zero |
| tourner90Droite() | Rotates exactly 90 degrees clockwise |
| tournerGauche() | Rotates exactly 90 degrees counter-clockwise |
| tracerCercleCentral() | Rotates 360 degrees to draw the center circle |
| initOLED() | Initializes the display |
| afficherMessage(l1, l2) | Displays a two-line message |

---

## Navigation Logic

### Distance Measurement

The robot does not use encoders. Distance is calculated using time:

```
Time (ms) = Distance (cm) / Speed (cm/s) x 1000
```

The speed value (43.33 cm/s) was measured empirically by running the robot for 3 seconds and recording the actual travel distance (130 cm).

### Straight Line Correction

Because the left and right motors never rotate at identical speeds, a calibration offset is applied:

```c
const int CALIBRATION_DROITE = -5;
```

- If the robot curves to the right, the value becomes more negative.
- If the robot curves to the left, the value becomes more positive.
- This offset is applied only to the forward movement function.

### Rotation Control

Rotations are controlled by the MPU6050 gyroscope rather than time:

1. The gyroscope angle is reset to zero.
2. Motors rotate in opposite directions.
3. The robot waits until the measured angle reaches the target.
4. Two speeds are used:
   - Fast speed until target minus 5 degrees.
   - Slow speed until the exact target.
5. A rotation calibration offset corrects systematic error:

```c
const int CALIBRATION_ROTATION = -7;
```

---

## Drawing Workflow

The field is drawn in seven sequential stages:

| Stage | Action | Distance/Time |
|-------|--------|---------------|
| 1 | Move half length | 75 cm |
| 2 | Rotate 90 degrees right | - |
| 3 | Move full width | 112.5 cm |
| 4 | Rotate 90 degrees right | - |
| 5 | Move full length | 150 cm |
| 6 | Rotate 90 degrees right | - |
| 7 | Move full width | 112.5 cm |
| 8 | Rotate 90 degrees right | - |
| 9 | Move half length | 75 cm |
| 10 | Rotate 90 degrees right | - |
| 11 | Move half length to center | 75 cm |
| 12 | Rotate 90 degrees right | - |
| 13 | Draw center line | 112.5 cm |
| 14 | Move backward to center | 56.25 cm |
| 15 | Rotate 90 degrees left | - |
| 16 | Draw center circle | 360 degrees |

---

## Execution Flow

```
Power On
    |
    v
Initialize Serial, I2C, MPU6050, Motors, OLED
    |
    v
Calibrate Gyroscope (200 samples, robot must be still)
    |
    v
Display "Starting in 3s"
    |
    v
Stage 1: Half length forward
    |
    v
Stage 2: Rotate right 90 degrees
    |
    v
Stages 3-10: Complete rectangle
    |
    v
Stages 11-12: Navigate to center
    |
    v
Stage 13: Draw center line
    |
    v
Stage 14: Return to center
    |
    v
Stage 15: Rotate to face drawing direction
    |
    v
Stage 16: Draw center circle (360 degrees)
    |
    v
Display "Complete"
    |
    v
End (infinite loop)
```

---

## Calibration Procedure

Before each competition or demonstration, the following must be performed:

### Step 1: Speed Calibration

1. Upload a simple test sketch that moves forward for 3 seconds.
2. Measure the actual distance traveled in centimeters.
3. Calculate the speed:

```
Speed (cm/s) = Distance (cm) / 3
```

4. Update `VITESSE_CM_S` and recalculate all timing constants.

### Step 2: Straight Line Calibration

1. Run the robot forward for 150 cm.
2. Observe the deviation:
   - Curves right: decrease `CALIBRATION_DROITE`.
   - Curves left: increase `CALIBRATION_DROITE`.
3. Repeat until the line is straight within 2 cm.

### Step 3: Rotation Calibration

1. Run a single 90-degree rotation.
2. Measure the final angle on the OLED display.
3. Adjust `CALIBRATION_ROTATION`:
   - If angle exceeds 90 degrees, decrease the value.
   - If angle is less than 90 degrees, increase the value.
4. Repeat for both left and right rotations.

---

## Known Issues and Limitations

### Issue 1: Gyroscope Calibration Difficulty

The MPU6050 required extensive calibration effort. The main challenges observed were:

- The gyroscope offset changes with temperature during operation.
- A single calibration at startup is not sufficient for the entire drawing session.
- The calibration procedure must be repeated before each run.
- Small variations in the offset value produce large cumulative errors over multiple rotations.

### Issue 2: Cumulative Rotation Error

Each 90-degree rotation introduces a small error of approximately 1 to 3 degrees (sometime more then that). Over the course of drawing the field, this error accumulates:

- After 4 rotations, the total deviation reaches 4 to 12 degrees.
- After 8 rotations, the deviation becomes clearly visible.
- The field gradually rotates and tilts on the drawing surface.

The primary consequence is that the final rectangle does not close properly, and the entire drawing appears skewed relative to the starting orientation.

### Issue 3: Inconsistent Rotation Angles

Empirical measurements from ten consecutive rotations showed the following final angles:

```
92, 95, 93, 98, 96, 98, 95, 92, 87, 89 degrees
```

This represents a variation of 11 degrees between the minimum and maximum. The causes are:

- Gyroscope integration error during fast rotation.
- Different friction on each wheel.

### Issue 4: Discrepancy Between Displayed and Actual Angle

The OLED displays the angle immediately after the rotation stops. However, when the robot begins the next movement, the actual angle has changed. This occurs because:

- The robot continues rotating slightly after motors stop (inertia).
- The gyroscope continues to integrate noise during the pause.

### Issue 5: Goal Rectangles Removed From Code

The original field design included goal area rectangles (5.5 m x 18.3 m real size, 6.9 cm x 22.9 cm at 1/80 scale). These were removed from the code for the following reasons:

- Drawing additional rectangles requires additional rotations.
- Each additional rotation adds more cumulative gyroscope error.
- By the time the robot reaches the goal area, the accumulated drift is too large.
- The resulting goal rectangles were drawn displaced from their correct positions.
- The overall drawing became messy and unusable.

The decision was made to keep only the main rectangle, center line, and center circle. These elements require fewer rotations and produce a recognizable field outline.

### Issue 6: No Real-Time Trajectory Correction

The robot does not correct its path during forward movement. The gyroscope is only used during rotations. A straight line is achieved only through motor calibration, not through active feedback control.

### Issue 7: Time-Based Distance Measurement

The robot does not use wheel encoders. Distance is estimated from time and a pre-calibrated speed. This causes:

- Battery voltage drop reduces motor speed over time.
- Surface friction variations change actual travel distance.
- Wheel slippage at startup causes the robot to travel less than expected.
- Cumulative error of 5 to 15 cm over the full field.

---

## Parameters Reference

| Parameter | Value | Description |
|-----------|-------|-------------|
| VITESSE_CM_S | 43.33 | Measured robot speed in cm/s |
| MOTOR_POWER | 80 | PWM value for forward movement |
| TURN_POWER_FAST | 100 | PWM for fast rotation phase |
| TURN_POWER_SLOW | 60 | PWM for slow rotation phase |
| CALIBRATION_DROITE | -5 | Straight line correction offset |
| CALIBRATION_ROTATION | -7 | Rotation angle correction offset |
| LONGUEUR | 150.0 | Field length in cm |
| LARGEUR | 112.5 | Field width in cm |

---

## Troubleshooting

| Symptom | Probable Cause | Solution |
|---------|---------------|----------|
| Robot does not move | Battery discharged | Recharge 18650 batteries |
| Robot moves in reverse | Motor wiring inverted | Swap motor wires or invert code logic |
| Line is curved | Motor imbalance | Adjust CALIBRATION_DROITE |
| Rotation not 90 degrees | Rotation calibration incorrect | Adjust CALIBRATION_ROTATION |
| Field tilts progressively | Cumulative gyroscope error | Recalibrate before each run; reduce number of rotations |
| OLED blank | I2C connection fault | Check A4/A5 wiring and address 0x3C |
| MPU6050 not found | Wiring or power issue | Check 5V, GND, SDA, SCL connections |
| Motors stall | Insufficient power | Verify ENA/ENB jumpers installed |
