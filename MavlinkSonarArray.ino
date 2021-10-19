/*  ======================================================================
 *  *** Mavlink Sonar Array ***
 *  
 *  File: MavlinkSonarArray.ino
 *  Description: Main ino file 
 *  
 *  MIT License  
 *  Copyright (c) 2021 Juan Benitez <juan.a.benitez(a)gmail.com>
 *  
 *  ======================================================================
 */

#include <Wire.h>

#include "qwiicSonar.h"
#include "mavlinkFunctions.h"

#define I2C_CLK_100MHZ  100*1000
#define I2C_CLK_400MHZ  400*1000

#define SONAR_PORT_FWD  7
#define SONAR_PORT_RGT  2
#define SONAR_PORT_LFT  5

long lastHB = 0;
long lastDistMsg = 0;
const long hbInterval = 1000;
const long distMsgInterval = 100;
 
qwiicSonar sonar[] { qwiicSonar(SONAR_PORT_FWD), 
                     qwiicSonar(SONAR_PORT_RGT),
                     qwiicSonar(SONAR_PORT_LFT) };

// #################################################################################
// SETUP
// #################################################################################
void setup() {
  Wire.begin();
  Wire.setClock( I2C_CLK_400MHZ );

  #define DEBUG_SERIAL_BAUD   115200
  #define MAVLINK_SERIAL_BAUD 57600

  Serial.begin( DEBUG_SERIAL_BAUD );
  Serial1.begin( MAVLINK_SERIAL_BAUD );
  delay( 3000 );
}


// #################################################################################
// LOOP
// #################################################################################
void loop() {

  //#define DEBUG_SONAR
  #define MAVLINK_ENABLED
  #define MEASURE_DELAY 100

  sonar[0].getDistance();
  sonar[1].getDistance();
  sonar[2].getDistance();
  delay(MEASURE_DELAY);


  //-----------------------------------------------------------
  // Debug data output
  //-----------------------------------------------------------
  #ifdef DEBUG_SONAR
  
  #define SONAR_ID 0  
  Serial.printf("RAW,MAVG,WAVG,MEDIAN,KF,\n%i,%i,%i,%i,%i\n", sonar[SONAR_ID].getFiltered(RAW), 
                                                              sonar[SONAR_ID].getFiltered(M_AVERAGE),
                                                              sonar[SONAR_ID].getFiltered(W_AVERAGE),
                                                              sonar[SONAR_ID].getFiltered(MEDIAN),,
                                                              sonar[SONAR_ID].getFiltered(KALMAN_FILTER,);
  #endif
  
  //-----------------------------------------------------------
  // Send mavlink data
  //-----------------------------------------------------------
  #ifdef MAVLINK_ENABLED

  #define MAVLINK_OUT_FILTER MEDIAN
  
  if ( millis() - lastHB >= hbInterval ) {
    sendMAVHertbeat();    
    lastHB = millis();
  }

  if ( millis() - lastDistMsg >= distMsgInterval ) {
    sendMAVSonar( MAV_SYSTEM_ID,  MAV_SENSOR_ROTATION_NONE,    sonar[0].getFiltered(MAVLINK_OUT_FILTER) );
    sendMAVSonar( MAV_SYSTEM_ID,  MAV_SENSOR_ROTATION_YAW_90,  sonar[1].getFiltered(MAVLINK_OUT_FILTER) );
    sendMAVSonar( MAV_SYSTEM_ID,  MAV_SENSOR_ROTATION_YAW_270, sonar[2].getFiltered(MAVLINK_OUT_FILTER) );

    lastDistMsg = millis();
  }
  
  #endif
}

// ##################################################################################################################################
