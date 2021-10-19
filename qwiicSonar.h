/*  ======================================================================
 *  *** Mavlink Sonar Array ***
 *  
 *  File: qwiicSonar.h
 *  Description: Qwiic Sonar class file
 *  
 *  MIT License  
 *  Copyright (c) 2021 Juan Benitez <juan.a.benitez(a)gmail.com>
 *  
 *  ======================================================================
 */
 
#pragma once

#include <Wire.h>
#include <stdint.h>

#include "sonarFilter.h"
#include "qwiicMux.h"

#define SONAR_ADDR 0x00
#define DEFAULT_MAX_DISTANCE 5000

enum filter_t {
  RAW,
  MEDIAN,
  M_AVERAGE,
  W_AVERAGE,  
  KALMAN_FILTER
};

class qwiicSonar {
  private:
    uint8_t muxPort;
    bool muxed = false;
    uint16_t maxDistance = DEFAULT_MAX_DISTANCE;

  public:
    qwiicMux mux;
    sonarFilter filter;    
    
  public:
    qwiicSonar();
    qwiicSonar(uint8_t muxPort);

    int16_t getDistance();
    int16_t getFiltered(filter_t ftype);
    void setMaxDistance(uint16_t);
};
