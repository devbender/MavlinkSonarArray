#pragma once

#include "c_library_v2/common/mavlink.h"

#define MAV_SYSTEM_ID 1

#define SONAR_MIN_DISTANCE 2
#define SONAR_MAX_DISTANCE 5000

#define SONAR_ID 1
#define SONAR_COVARIANCE 255

#define SONAR_HOR_FOV 15.00
#define SONAR_VER_FOC 5.00

#define SONAR_SIGNAL_QUALITY 100
#define SONAR_QUATERNION {0.00, 0.00, 0.00, 0.00}

void sendMAVHertbeat();
void sendMAVSonar(uint8_t SYSTEM_ID, MAV_SENSOR_ORIENTATION SONAR_ORIENTATION, uint16_t SONAR_DISTANCE_MM);

//==============================================================
// Send mavlink heartbeat msg
//==============================================================
void sendMAVHertbeat() {
  mavlink_message_t mav_heartbeat_message;
  uint8_t mav_heartbeat_buffer[MAVLINK_MAX_PACKET_LEN];

  // Pack mavlink message
  mavlink_msg_heartbeat_pack(MAV_SYSTEM_ID,
                             MAV_COMP_ID_OBSTACLE_AVOIDANCE,
                             &mav_heartbeat_message,
                             MAV_TYPE_GENERIC,
                             MAV_AUTOPILOT_GENERIC,
                             0, 0,
                             MAV_STATE_ACTIVE);

  // Send packed message to buffer
  uint16_t len = mavlink_msg_to_send_buffer(mav_heartbeat_buffer, &mav_heartbeat_message);
  Serial.write(mav_heartbeat_buffer, len);
  Serial1.write(mav_heartbeat_buffer, len);
}


//==============================================================
// MAVLINK SONAR OUT
//==============================================================
void sendMAVSonar(uint8_t SYSTEM_ID, MAV_SENSOR_ORIENTATION SONAR_ORIENTATION, uint16_t SONAR_DISTANCE_MM) {

  uint16_t SONAR_DISTANCE_CM = round( SONAR_DISTANCE_MM  / 10 );

  mavlink_message_t msg;
  uint8_t mav_buffer[MAVLINK_MAX_PACKET_LEN];
  float quaternion[4] = SONAR_QUATERNION;

  mavlink_msg_distance_sensor_pack( SYSTEM_ID,
                                    MAV_COMP_ID_OBSTACLE_AVOIDANCE,
                                    &msg,
                                    0,
                                    SONAR_MIN_DISTANCE,
                                    SONAR_MAX_DISTANCE,
                                    SONAR_DISTANCE_CM,
                                    MAV_DISTANCE_SENSOR_ULTRASOUND,
                                    SONAR_ID,
                                    SONAR_ORIENTATION,
                                    SONAR_COVARIANCE,
                                    SONAR_HOR_FOV,
                                    SONAR_VER_FOC,
                                    &quaternion[0],
                                    SONAR_SIGNAL_QUALITY);

  uint16_t len = mavlink_msg_to_send_buffer(mav_buffer, &msg);
  Serial.write(mav_buffer, len);
  Serial1.write(mav_buffer, len);
}
