/*  ======================================================================
 *  *** Mavlink Sonar Array ***
 *  
 *  File: sonarFilter.h
 *  Description: Qwiic Sonar filtering class file
 *  
 *  MIT License
 *  Copyright (c) 2021 Juan Benitez <juan.a.benitez(a)gmail.com>
 *  
  * Kalman Filter implementation extracted from:
 * https://github.com/denyssene/SimpleKalmanFilter
 * 
 * MIT License
 * Copyright (c) 2017 Denys Sene
 *  
 *  ======================================================================
 */
 
#pragma once

#include <math.h>
#include <vector>
#include <algorithm>
#include <stdint.h>

/*  
 * Kalman Filter implementation extracted from:
 * https://github.com/denyssene/SimpleKalmanFilter
 * 
 * MIT License
 * Copyright (c) 2017 Denys Sene
 * 
 */

// #################################################################################
// SONAR FILTERING CLASS
// #################################################################################
class sonarFilter {

  private:
    uint8_t samples = 5;
    uint16_t d_raw, d_maverage, d_median, d_mixed, d_wavg, d_kalmanFilter;
    std::vector<uint16_t> data, dataCopy;    
    float wp1=0.4, wp2=0.4, wp3=0.2;

    // Kalman filer parameters
    float _err_measure = 1;
    float _err_estimate = 1;
    float _q = 0.01;
    float _current_estimate;
    float _last_estimate;
    float _kalman_gain;


  public:
    sonarFilter(){};
    sonarFilter(uint8_t _samples);

    void setSamples(uint8_t _samples);
    void setKFParams(float mea_e, float est_e, float q);
    void setWAParams(float _wp1, float _wp2, float _wp3);

    void add(uint16_t measurement);
    float updateKFEstimate(float mea);

    uint16_t raw();
    uint16_t mavg();
    uint16_t wavg();
    uint16_t median();
    uint16_t kalman();
};
