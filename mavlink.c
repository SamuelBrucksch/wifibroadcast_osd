#include "mavlink.h"

#ifdef MAVLINK
mavlink_status_t status;
mavlink_message_t msg;


void mavlink_read(telemetry_data_t *td, uint8_t *buf, int buflen) {
	int i;
	for(i=0; i<buflen; i++) {
		uint8_t c = buf[i];
		if (mavlink_parse_char(0, c, &msg, &status)) {
                	switch (msg.msgid){
                        	case MAVLINK_MSG_ID_GPS_RAW_INT:
					td->fix = mavlink_msg_gps_raw_int_get_fix_type(&msg);
					td->sats = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
					break;
                                case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
					td->heading = mavlink_msg_global_position_int_get_hdg(&msg)/100.0f;
                                        td->altitude = mavlink_msg_global_position_int_get_relative_alt(&msg)/1000.0f;
                                        td->latitude = mavlink_msg_global_position_int_get_lat(&msg)/10000000.0f;
                                        td->longitude = mavlink_msg_global_position_int_get_lon(&msg)/10000000.0f;
                                        break;
                                case MAVLINK_MSG_ID_ATTITUDE:
					td->roll = mavlink_msg_attitude_get_roll(&msg)*57.2958;
					td->pitch = mavlink_msg_attitude_get_pitch(&msg)*57.2958;
					break;
                                case MAVLINK_MSG_ID_SYS_STATUS:
					td->voltage = mavlink_msg_sys_status_get_voltage_battery(&msg)/1000.0f;
					td->ampere = mavlink_msg_sys_status_get_current_battery(&msg)/100.0f;
                                        break;
                                case MAVLINK_MSG_ID_VFR_HUD:
                                        td->speed = mavlink_msg_vfr_hud_get_groundspeed(&msg)*3.6f;
                                        break;
        	}
	}
  }
}
#endif
