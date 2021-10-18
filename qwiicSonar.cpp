#include "qwiicSonar.h"

qwiicSonar::qwiicSonar(uint8_t port) {
  muxPort = port;
  muxed = true;
}


int16_t qwiicSonar::getDistance() {

  if(muxed) mux.setPort( muxPort );

  uint8_t distance_H = 0;
  uint8_t distance_L = 0;
  int16_t distance = 0;

  Wire.beginTransmission(SONAR_ADDR); // transmit to device #8
  Wire.write(1);                      // measure command: 0x01
  Wire.endTransmission();             // stop transmitting

  Wire.requestFrom(SONAR_ADDR, 2);
  if (Wire.available()) {
    distance_H = Wire.read();
    distance_L = Wire.read();
    distance = (int16_t)distance_H << 8;
    distance = distance | distance_L;

    if (distance > maxDistance) {
      filter.add( distance );
      return maxDistance;
    }      
    else {
      filter.add( distance );
      return distance;
    }
  }

  return -1;
}


int16_t qwiicSonar::getFiltered(filter_t ftype) { 

  switch(ftype) {
    
    case RAW: 
      return filter.raw(); 
      break;
      
    case M_AVERAGE: 
      return filter.mavg(); 
      break;
      
    case W_AVERAGE: 
      return filter.wavg(); 
      break;
      
    case MEDIAN: 
      return filter.median(); 
      break;    

    case KALMAN_FILTER: 
      return filter.kalman(); 
      break;
      
    default: 
      return filter.raw(); 
      break;
  }

  return -1;
}

void qwiicSonar::setMaxDistance(uint16_t _maxDistance) {
  maxDistance = _maxDistance;
}
