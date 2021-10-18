#pragma once

#include <Wire.h>
#include <stdint.h>

#define QWIIC_MUX_DEFAULT_ADDRESS 0x70

typedef bool boolean;

class qwiicMux {

  private:
    int deviceAddress = QWIIC_MUX_DEFAULT_ADDRESS;

  public:
    qwiicMux(){};    

    boolean init();
    uint8_t setPort(uint8_t portNumber);
    
    void setPortState(uint8_t portBits);
    int getPort();
    uint8_t getPortState();

    void enablePort(uint8_t portNumber);
    void disablePort(uint8_t portNumber);
};
