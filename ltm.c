/* #################################################################################################################
 * LightTelemetry protocol (LTM)
 *
 * Ghettostation one way telemetry protocol for really low bitrates (1200/2400 bauds). 
 *			   
 * Protocol details: 3 different frames, little endian.
 *   G Frame (GPS position) (2hz @ 1200 bauds , 5hz >= 2400 bauds): 18BYTES
 *    0x24 0x54 0x47 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF  0xFF   0xC0   
 *     $     T    G  --------LAT-------- -------LON---------  SPD --------ALT-------- SAT/FIX  CRC
 *   A Frame (Attitude) (5hz @ 1200bauds , 10hz >= 2400bauds): 10BYTES
 *     0x24 0x54 0x41 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xC0   
 *      $     T   A   --PITCH-- --ROLL--- -HEADING-  CRC
 *   S Frame (Sensors) (2hz @ 1200bauds, 5hz >= 2400bauds): 11BYTES
 *     0x24 0x54 0x53 0xFF 0xFF  0xFF 0xFF    0xFF    0xFF      0xFF       0xC0     
 *      $     T   S   VBAT(mv)  Current(ma)   RSSI  AIRSPEED  ARM/FS/FMOD   CRC
 * ################################################################################################################# */
#include "ltm.h"
#ifdef LTM
  static uint8_t LTMserialBuffer[LIGHTTELEMETRY_GFRAMELENGTH-4];
  static uint8_t LTMreceiverIndex;
  static uint8_t LTMcmd;
  static uint8_t LTMrcvChecksum;
  static uint8_t LTMreadIndex;
  static uint8_t LTMframelength;



uint8_t ltmread_u8()  {
  return LTMserialBuffer[LTMreadIndex++];
}

uint16_t ltmread_u16() {
  uint16_t t = ltmread_u8();
  t |= (uint16_t)ltmread_u8()<<8;
  return t;
}

uint32_t ltmread_u32() {
  uint32_t t = ltmread_u16();
  t |= (uint32_t)ltmread_u16()<<16;
  return t;
}

static enum _serial_state {
    IDLE,
    HEADER_START1,
    HEADER_START2,
    HEADER_MSGTYPE,
    HEADER_DATA
}
c_state = IDLE;

void ltm_read(telemetry_data_t *td, uint8_t *buf, int buflen) {
  int i;
  for(i=0; i<buflen; ++i) {
    uint8_t c = buf[i];
    if (c_state == IDLE) {
      c_state = (c=='$') ? HEADER_START1 : IDLE;
        //Serial.println("header $" );
    }
    else if (c_state == HEADER_START1) {
      c_state = (c=='T') ? HEADER_START2 : IDLE;
        //Serial.println("header T" );
    }
    else if (c_state == HEADER_START2) {
      switch (c) {
         case 'G':
           LTMframelength = LIGHTTELEMETRY_GFRAMELENGTH;
           c_state = HEADER_MSGTYPE;
           break;
         case 'A':
           LTMframelength = LIGHTTELEMETRY_AFRAMELENGTH;
           c_state = HEADER_MSGTYPE;
           break;
         case 'S':
           LTMframelength = LIGHTTELEMETRY_SFRAMELENGTH;
           c_state = HEADER_MSGTYPE;
           break;
         default:
           c_state = IDLE;
      }
      LTMcmd = c;
      LTMreceiverIndex=0;
    }
    else if (c_state == HEADER_MSGTYPE) {
	  if(LTMreceiverIndex == 0) {
	  LTMrcvChecksum = c;
	  } 
	  else {
	  LTMrcvChecksum ^= c;
	  }
      if(LTMreceiverIndex == LTMframelength-4) {   // received checksum byte
        if(LTMrcvChecksum == 0) {
	    	//telemetry_ok = 1;
            	//TODO
		//lastpacketreceived = millis();
            ltm_check(td);
            c_state = IDLE;
        }
        else {                                                   // wrong checksum, drop packet
        c_state = IDLE; 
        
        }
      }
      else LTMserialBuffer[LTMreceiverIndex++]=c;
  }
	}
}

// --------------------------------------------------------------------------------------
// Decoded received commands 
void ltm_check(telemetry_data_t *td) {
  LTMreadIndex = 0;
                    
  if (LTMcmd==LIGHTTELEMETRY_GFRAME)  {
    td->latitude = (double)((int32_t)ltmread_u32())/10000000;
    td->longitude = (double)((int32_t)ltmread_u32())/10000000;
    uint8_t uav_groundspeedms = ltmread_u8();
    td->speed = (float)(uav_groundspeedms * 3.6f); // convert to kmh
    td->altitude = (float)((int32_t)ltmread_u32())/100.0f;
    uint8_t ltm_satsfix = ltmread_u8();
    td->sats = (ltm_satsfix >> 2) & 0xFF;
    td->fix = ltm_satsfix & 0b00000011;
    //memset(LTMserialBuffer, 0, LIGHTTELEMETRY_GFRAMELENGTH-4); 
  }else if (LTMcmd==LIGHTTELEMETRY_AFRAME)  {
    td->pitch = (int16_t)ltmread_u16();
    td->roll =  (int16_t)ltmread_u16();
    td->heading = (float)((int16_t)ltmread_u16());
    if (td->heading < 0 ) td->heading = td->heading + 360; //convert from -180/180 to 0/360°
    //memset(LTMserialBuffer, 0, LIGHTTELEMETRY_AFRAMELENGTH-4); 
  }else if (LTMcmd==LIGHTTELEMETRY_SFRAME)  {
    td->voltage = (float)ltmread_u16()/1000.0f;
    td->ampere = (float)ltmread_u16()/1000.0f;
    td->rssi = ltmread_u8();
    td->airspeed = ltmread_u8();
    uint8_t ltm_armfsmode = ltmread_u8();
    //TODO flight modes
    //uav_arm = ltm_armfsmode & 0b00000001;
    //uav_failsafe = (ltm_armfsmode >> 1) & 0b00000001;
    //uav_flightmode = (ltm_armfsmode >> 2) & 0b00111111;
    //memset(LTMserialBuffer, 0, LIGHTTELEMETRY_SFRAMELENGTH-4);     
  }
}
#endif
