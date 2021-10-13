#pragma once

#include "qwiicMuxFx.h"

enum sonar_t {
  SONAR_FWD,
  SONAR_RGT,
  SONAR_LFT,
  SONAR_BCK,
  SONAR_DWN  
};

struct sonars {
  uint16_t fwd, rgt, lft, bck, dwn;
};

uint16_t sonarRead();
uint16_t sonarReadPort(uint8_t port);

//==============================================================
// GET SONAR DISTANCE
//==============================================================
uint16_t sonarRead() {

  uint8_t distance_H = 0;
  uint8_t distance_L = 0;
  uint16_t distance = 0;

  Wire.beginTransmission(SONAR_ADDR); // transmit to device #8
  Wire.write(1);                      // measure command: 0x01
  Wire.endTransmission();             // stop transmitting

  Wire.requestFrom(SONAR_ADDR, 2);
  if (Wire.available()) {
    distance_H = Wire.read();
    distance_L = Wire.read();
    distance = (uint16_t)distance_H << 8;
    distance = distance | distance_L;

    if (distance > MAX_DISTANCE)
      return MAX_DISTANCE;
    else
      return distance;
  }

  return MAX_DISTANCE;
}


//==============================================================
// GET SONAR DISTANCE FROM MUX PORT
//==============================================================
uint16_t sonarReadPort(uint8_t port) {

  MUXsetPort( port );

  uint8_t distance_H = 0;
  uint8_t distance_L = 0;
  uint16_t distance = 0;

  Wire.beginTransmission(SONAR_ADDR); // transmit to device #8
  Wire.write(1);                      // measure command: 0x01
  Wire.endTransmission();             // stop transmitting

  Wire.requestFrom(SONAR_ADDR, 2);
  if (Wire.available()) {
    distance_H = Wire.read();
    distance_L = Wire.read();
    distance = (uint16_t)distance_H << 8;
    distance = distance | distance_L;

    if (distance > MAX_DISTANCE)
      return MAX_DISTANCE;
    else
      return distance;
  }

  return MAX_DISTANCE;
}
