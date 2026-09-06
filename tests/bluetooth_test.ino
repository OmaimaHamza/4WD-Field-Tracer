#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX, TX

void setup()
{
  Serial.begin(9600);
  BT.begin(9600);

  Serial.println("HC-06 Ready");
  BT.println("Bluetooth Connected!");
}

void loop()
{
  // Data from Bluetooth App to Serial Monitor
  if (BT.available())
  {
    char c = BT.read();
    Serial.write(c);
  }

  // Data from Serial Monitor to Bluetooth App
  if (Serial.available())
  {
    char c = Serial.read();
    BT.write(c);
  }
}