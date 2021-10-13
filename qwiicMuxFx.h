#pragma once

int _deviceAddress;

#define QWIIC_MUX_DEFAULT_ADDRESS 0x70

boolean MUXisConnected();
boolean MUXbegin(uint8_t deviceAddress);
uint8_t MUXsetPort(uint8_t portNumber);
boolean MUXsetPortState(uint8_t portBits);
uint8_t MUXgetPort();
uint8_t MUXgetPortState();
boolean MUXenablePort(uint8_t portNumber);
boolean MUXdisablePort(uint8_t portNumber);

// =============================================================
// IS CONNECTED
// =============================================================
boolean MUXisConnected() {

  uint8_t write = 0;
  Wire.beginTransmission(_deviceAddress);
  Wire.write(write);
  Wire.endTransmission();

  Wire.requestFrom(_deviceAddress, 1);
  if (Wire.available()) {
    uint8_t read = Wire.read();

    if (read != 0)
      return false;
  }

  MUXsetPortState(0xA4);
  uint8_t response = MUXgetPortState();
  MUXsetPortState(0x00);

  if (response == 0xA4)
    return true;
  return false;
}


// =============================================================
// BEGIN
// =============================================================
boolean MUXbegin(uint8_t deviceAddress) {

  _deviceAddress = deviceAddress;

  if ( !MUXisConnected() ) return (false);
  return (true);
}


// =============================================================
// SET PORT
// =============================================================
uint8_t MUXsetPort(uint8_t portNumber) {

  uint8_t portValue = 0;

  if (portNumber > 7)
    portValue = 0;
  else
    portValue = 1 << portNumber;

  Wire.beginTransmission(_deviceAddress);
  Wire.write(portValue);
  Wire.endTransmission();

  return MUXgetPort();
}


// =============================================================
// GET PORT
// =============================================================
uint8_t MUXgetPort() {

  uint8_t portBits;

  Wire.requestFrom(_deviceAddress, 1);
  if (Wire.available()) {
    portBits = Wire.read();

    for (uint8_t x = 0; x < 8; x++) {
      if (portBits & (1 << x))
        return x;
    }
  }

  return 255;
}


// =============================================================
// SET PORT STATE
// =============================================================
boolean MUXsetPortState(uint8_t portBits) {

  Wire.beginTransmission(_deviceAddress);
  Wire.write(portBits);
  Wire.endTransmission();

  return true;
}


// =============================================================
// GET PORT STATE
// =============================================================
uint8_t MUXgetPortState() {

  uint8_t portBits = 0;

  Wire.requestFrom(_deviceAddress, 1);
  if (Wire.available())
    portBits = Wire.read();

  return portBits;
}


// =============================================================
// ENABLE PORT
// =============================================================
boolean MUXenablePort(uint8_t portNumber) {

  if (portNumber > 7) portNumber = 7;

  uint8_t settings = MUXgetPortState();
  settings |= (1 << portNumber);

  return MUXsetPortState(settings);
}


// =============================================================
// DISABLE PORT
// =============================================================
boolean MUXdisablePort(uint8_t portNumber) {

  if (portNumber > 7) portNumber = 7;

  uint8_t settings = MUXgetPortState();
  settings &= ~(1 << portNumber);

  return MUXsetPortState(settings);
}
