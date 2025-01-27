/*
  Using the BNO080 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the parts of the calibrated gyro.

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO080 breakout to avoid polling.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h"
BNO080 myIMU;
float x = 0;
float y = 0;
float z = 0;
#define Pi 3.14159 
unsigned long new_time;
unsigned long old_time;
unsigned int dtime;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("BNO080 Read Example");

  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  myIMU.begin();

  myIMU.enableGyro(200); //Send data update every 50ms

  Serial.println(F("Gyro enabled"));
  Serial.println(F("Output in form x, y, z, in radians per second"));
}

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    float p = myIMU.getGyroX();
    float q = myIMU.getGyroY();
    float r = myIMU.getGyroZ();

    new_time = millis();

    float xdot = p + q * sin(x) * cos(y) + r * cos(x) * tan(y);
    float ydot = q * cos(x) - r * sin(x);
    float zdot = q * sin(x) / cos(y) + r * cos(x) / cos(y);

    dtime = new_time - old_time;
    x = (x + xdot * dtime) % (2 * Pi);
    dtime = new_time - old_time;
    y = (y + ydot * dtime) % (2 * Pi);
    dtime = new_time - old_time;
    z = (z + zdot * dtime) % (2 * Pi);

    old_time = new_time;

    Serial.print(x, 2);
    Serial.print(F(","));
    Serial.print(y, 2);
    Serial.print(F(","));
    Serial.print(z, 2);
    Serial.print(F(","));
    Serial.println();
  }
}

