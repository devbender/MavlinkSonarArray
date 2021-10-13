#include <Wire.h>

#include "filterClass.h"
#include "sonarParams.h"
#include "qwiicSonarFx.h"
#include "qwiicMuxFx.h"
#include "mavlinkFx.h"

#define I2C_CLK_100MHZ  100*1000
#define I2C_CLK_400MHZ  400*1000

#define SONAR_PORT_FWD  7
#define SONAR_PORT_RGT  2
#define SONAR_PORT_LFT  5
#define SONAR_PORT_DWN  3

const uint8_t sonarPorts[] = { SONAR_PORT_FWD, SONAR_PORT_RGT, SONAR_PORT_LFT};
const uint8_t NUM_SONARS = sizeof(sonarPorts)-1;

long lastHB = 0;
const long hbInterval = 1000;
 
sonars s;
sonarfilter filter[3];

// #################################################################################
// SETUP
// #################################################################################
void setup() {
  Wire.begin();
  Wire.setClock( I2C_CLK_400MHZ );

  Serial.begin( 115200 );
  Serial1.begin( 57600 );
  delay( 3000 );

  if ( !MUXbegin( QWIIC_MUX_DEFAULT_ADDRESS ) ) 
    Serial.println( ">> MUX NOT CONNECTED!");
  else {
    Serial.print(">> MUX PORT SET TO: "); 
    Serial.println( MUXsetPort(0) );
  }
}


// #################################################################################
// LOOP
// #################################################################################
void loop() {

  //-----------------------------------------------------------
  // Send mavlink heartbeat message
  //-----------------------------------------------------------
  if ( millis() - lastHB >= hbInterval ) {
    sendMAVHertbeat();
    lastHB = millis();
  }


  //-----------------------------------------------------------
  // Send mavlink distance message
  //-----------------------------------------------------------

  #define MEASURE_DELAY 25

  filter[0].add( sonarReadPort( SONAR_PORT_FWD ) ); delay( MEASURE_DELAY );
  filter[1].add( sonarReadPort( SONAR_PORT_RGT ) ); delay( MEASURE_DELAY );
  filter[2].add( sonarReadPort( SONAR_PORT_LFT ) ); delay( MEASURE_DELAY );  

  s.fwd = filter[0].wavg();
  s.rgt = filter[1].wavg();
  s.lft = filter[2].wavg();
  //s.bck = SONAR_MAX_DISTANCE;
  //s.dwn = SONAR_MAX_DISTANCE;

  //Serial.println("FWD,RGT,LFT");
  //Serial.printf("%i,%i,%i\n", s.fwd, s.rgt, s.lft);
  
  sendMavSonars( &s );
  

  /*
  //------------------------------------------------------------------------
  // Read single sonar
  //------------------------------------------------------------------------
  uint16_t raw = sonarRead();
  filter[SONAR_FWD].add( raw );

  Serial.println("RAW,KF,WAVG");
  Serial.printf("%i,%i,%i\n", raw, filter[SONAR_FWD].kalman(), filter[SONAR_FWD].wavg() );
  delay( 100 );
  */

}

// ##################################################################################################################################
