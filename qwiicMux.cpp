#include "qwiicMux.h"

boolean qwiicMux::init() {
  uint8_t write = 0;
  Wire.beginTransmission(deviceAddress);
  Wire.write(write);
  Wire.endTransmission();
  
  Wire.requestFrom(deviceAddress, 1);
  if (Wire.available()) {  
    if (Wire.read() != 0)
      return false;
  }
  
  setPortState(0xA4);
  uint8_t response = getPortState();
  setPortState(0x00);
  
  if (response == 0xA4)
    return true;
   else
    return false;
}


uint8_t qwiicMux::setPort(uint8_t portNumber) {
  uint8_t portValue = 0;

  if (portNumber > 7)
    portValue = 0;
  else
    portValue = 1 << portNumber;

  Wire.beginTransmission(deviceAddress);
  Wire.write(portValue);
  Wire.endTransmission();

  return getPort();
}


void qwiicMux::setPortState(uint8_t portBits) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(portBits);
  Wire.endTransmission();  
}


int qwiicMux::getPort() {
  
  uint8_t portBits;

  Wire.requestFrom(deviceAddress, 1);
  if (Wire.available()) {
    portBits = Wire.read();

    for (uint8_t x = 0; x < 8; x++) {
      if (portBits & (1 << x))
        return x;
    }
  }
  return -1; 
}


uint8_t qwiicMux::getPortState() {
  uint8_t portBits = 0;

  Wire.requestFrom(deviceAddress, 1);
  if (Wire.available())
    portBits = Wire.read();

  return portBits;
}


void qwiicMux::enablePort(uint8_t portNumber) {
  if (portNumber > 7) portNumber = 7;

  uint8_t settings = getPortState();
  settings |= (1 << portNumber);

  setPortState(settings);
}


void qwiicMux::disablePort(uint8_t portNumber) {
  if (portNumber > 7) portNumber = 7;

  uint8_t settings = getPortState();
  settings &= ~(1 << portNumber);

  setPortState(settings);
}
