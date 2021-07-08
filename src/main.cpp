#include <Arduino.h>
#include <Wire.h>

#include "TCA9548A.h"
#include "c_library_v2/common/mavlink.h"

TCA9548A mux;

#define I2C_SDA 16
#define I2C_SCL 17
#define I2C_CLK_100MHZ  100*1000
#define I2C_CLK_400MHZ  400*1000

MbedI2C i2c(I2C_SDA, I2C_SCL);

void setup() {
  i2c.begin();
  i2c.setClock(I2C_CLK_400MHZ);
  Serial.begin(115200);
  Serial1.begin(57600);

  mux.begin(i2c);
  mux.closeAll();
}

void loop() {
  // put your main code here, to run repeatedly:
}