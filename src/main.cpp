#include <Arduino.h>
#include <Wire.h>

#include "TCA9548A.h"
#include "c_library_v2/common/mavlink.h"

#define I2C_SDA 16
#define I2C_SCL 17
#define I2C_CLK_100MHZ  100*1000
#define I2C_CLK_400MHZ  400*1000

TCA9548A mux;
MbedI2C i2c(I2C_SDA, I2C_SCL);

void sendMAVSonar(uint8_t , MAV_SENSOR_ORIENTATION , uint16_t); 

//==============================================================
// SETUP
//==============================================================
void setup() {
  i2c.begin();
  i2c.setClock(I2C_CLK_400MHZ);
  Serial.begin(115200);
  Serial1.begin(57600);

  mux.begin(i2c);
  mux.closeAll();
}

//==============================================================
// LOOP
//==============================================================
void loop() {
  Serial.println("Hello World!");
  delay(1000);
}



//==============================================================
// MAVLINK SONAR
//==============================================================
void sendMAVSonar(uint8_t SYSTEM_ID, MAV_SENSOR_ORIENTATION SONAR_ORIENTATION, uint16_t SONAR_DISTANCE_MM) {

  uint16_t SONAR_DISTANCE_CM = round( SONAR_DISTANCE_MM ) / 10;
  
  mavlink_message_t msg;
  uint8_t mav_buffer[MAVLINK_MAX_PACKET_LEN];
  
  uint16_t min_distance = 2;
  uint16_t max_distance = 400;
  uint8_t id = 1;
  uint8_t covariance = 255;
  float horizontal_fov = 25.00;
  float vertical_fov = 5.00;
  float quaternion[4] = {0.00, 0.00, 0.00, 0.00};
  uint8_t signal_quality = 100;

  mavlink_msg_distance_sensor_pack( SYSTEM_ID,
                              MAV_COMP_ID_OBSTACLE_AVOIDANCE,
                              &msg,
                              0,
                              min_distance,
                              max_distance,
                              SONAR_DISTANCE_CM,
                              MAV_DISTANCE_SENSOR_ULTRASOUND,
                              id,
                              SONAR_ORIENTATION,
                              covariance,
                              horizontal_fov,
                              vertical_fov,
                              &quaternion[0],
                              signal_quality);

  uint16_t len = mavlink_msg_to_send_buffer(mav_buffer, &msg);
  Serial1.write(mav_buffer, len);
}