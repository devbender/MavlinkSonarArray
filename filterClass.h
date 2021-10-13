#pragma once

#include <math.h>
#include <vector>
#include <algorithm>

// #################################################################################
// MOVING AVERAGE FILTER
// #################################################################################
class sonarfilter {

  private:
    uint8_t samples = 5;
    uint16_t d_maverage, d_median, d_mixed, d_wavg, d_kalmanFilter;
    std::vector<uint16_t> data;
    std::vector<uint16_t> dataOUT;
    float wp1=0.4, wp2=0.4, wp3=0.2;

    // Kalman filer
    float _err_measure = 1;
    float _err_estimate = 1;
    float _q = 0.01;
    float _current_estimate;
    float _last_estimate;
    float _kalman_gain;
    

  public:
    // CONSTRUCTORS =======================================================
    sonarfilter() { }    

    sonarfilter(uint8_t _samples) {
      samples = _samples;
    }
    

    // SETTERS ============================================================
    void setSamples(uint8_t _samples) {
      samples = _samples;      
    }

    void setKFParams(float mea_e, float est_e, float q) {
      _err_measure=mea_e;
      _err_estimate=est_e;
      _q = q;
    }

    void setWAParams(float _wp1, float _wp2, float _wp3) {
      if( _wp1+_wp2+_wp3 > 1) {
        while(1) {
          Serial.println(">> ERR: Weights must not exceed 100%");
          delay(5000);
        }
      }

      wp1=_wp1;
      wp2=_wp2;
      wp3=_wp3;
    }

    // KF UPDATE============================================================
    float updateEstimate(float mea) {

      _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
      _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
      _err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
      _last_estimate=_current_estimate;
    
      return _current_estimate;
    }


    // ADD MEASUREMENT ====================================================
    void add(uint16_t newSample) {

      // add sample to pool
      data.push_back( newSample );

      // delete oldest
      if( data.size() > samples )
        data.erase( data.begin() );

      
      // Average ----------------------------------
      uint16_t sumSamples = 0;

      for(auto& sample : data)
        sumSamples+=sample;

      d_maverage = round( sumSamples / data.size() );


      // Median -----------------------------------
      if(data.size() >= samples) {       

        dataOUT = data;
        std::sort(dataOUT.begin(), dataOUT.end());
        d_median = dataOUT[ dataOUT.size()/2 ];
      }
      
      // Kalman -----------------------------------
      d_kalmanFilter = updateEstimate( newSample );

      // Mixed ------------------------------------
      d_mixed = round( (d_maverage + d_median)/2 );

      // Weighted Avg -----------------------------
      d_wavg = round( (wp1)*d_maverage + (wp2)*d_median + (wp3)*d_kalmanFilter  );
    }


    // GETTERS ============================================================
    uint16_t mavg() {
      return d_maverage;
    }

    uint16_t median() {
      return d_median;
    }

    uint16_t avgmix() {      
      return d_mixed;
    }

    uint16_t wavg() {      
      return d_wavg;
    }

    uint16_t kalman() {
      return d_kalmanFilter;
    }

};
