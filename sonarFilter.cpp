#include "sonarFilter.h"

sonarFilter::sonarFilter(uint8_t _samples) {
      samples = _samples;
}


void sonarFilter::setSamples(uint8_t _samples) {
  samples = _samples;
}


void sonarFilter::setKFParams(float mea_e, float est_e, float q) {
  _err_measure=mea_e;
  _err_estimate=est_e;
  _q = q;
}


void sonarFilter::setWAParams(float _wp1, float _wp2, float _wp3) {
  wp1=_wp1;
  wp2=_wp2;
  wp3=_wp3;
}


float sonarFilter::updateKFEstimate(float mea) {

  _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
  _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
  _err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
  _last_estimate=_current_estimate;

  return _current_estimate;
}


void sonarFilter::add(uint16_t newSample) {

  // store raw sample
  d_raw = newSample;

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

    dataCopy = data;
    std::sort(dataCopy.begin(), dataCopy.end());
    d_median = dataCopy[ round(dataCopy.size()/2) ];
  }
  
  // Kalman -----------------------------------
  d_kalmanFilter = updateKFEstimate( newSample );

  // Weighted Avg -----------------------------
  d_wavg = round( (wp1)*d_maverage + (wp2)*d_median + (wp3)*d_kalmanFilter  );
}


uint16_t sonarFilter::raw() {
  return d_raw;
}


uint16_t sonarFilter::mavg() {
  return d_maverage;
}


uint16_t sonarFilter::wavg() {      
  return d_wavg;
}


uint16_t sonarFilter::median() {
  return d_median;
}


uint16_t sonarFilter::kalman() {
  return d_kalmanFilter;
}
