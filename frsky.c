#include <stdint.h>
#include "telemetry.h"
#include "frsky.h"

int frsky_parse_buffer(frsky_state_t *state, telemetry_data_t *td, uint8_t *buf, int buflen) {
	int new_data = 0;
	int i;
	for(i=0; i<buflen; ++i) {
		uint8_t ch = buf[i];
		switch(state->sm_state) {
			case 0:
				if(ch == 0x5e)
					state->sm_state = 1;
				break;
			case 1:
				if(ch == 0x5e)
					state->sm_state = 2;
				else
					state->sm_state = 0;
				break;
			case 2:
				if(ch == 0x5e) {
					state->pkg_pos = 0;
					new_data = new_data | frsky_interpret_packet(state, td);
				}
				else {
					if(state->pkg_pos >= sizeof(state->pkg)) {
						state->pkg_pos = 0;
						state->sm_state = 0;
					} else {
						state->pkg[state->pkg_pos] = ch;
						state->pkg_pos++;
					}
				}
				break;
			default:
				state->sm_state = 0;
			break;
		}
	}
	return new_data;
}

int frsky_interpret_packet(frsky_state_t *state, telemetry_data_t *td) {
	uint16_t data;
	int new_data = 1;

	data = *(uint16_t*)(state->pkg+1);
	switch(state->pkg[0]) {
		case ID_VOLT:
		{
			uint16_t val = (state->pkg[2] >> 8) | ((state->pkg[1] & 0xf) << 8);
			float battery = 3.0 * val / 500;
			td->voltage = battery;
		}
			break;
		case ID_GPS_ALTIDUTE_BP:
			td->altitude = data;
			break;
		case ID_LONGITUDE_BP:
			td->longitude = data / 100;
			td->longitude += 1.0 * (data - td->longitude * 100) / 60;
			break;
		case ID_LONGITUDE_AP:
			td->longitude +=  1.0 * data / 60 / 10000;
			break;
		case ID_LATITUDE_BP:
			td->latitude = data / 100;
			td->latitude += 1.0 * (data - td->latitude * 100) / 60;
			break;
		case ID_LATITUDE_AP:
			td->latitude +=  1.0 * data / 60 / 10000;
			break;
		case ID_COURSE_BP:
			td->heading = (int16_t)data;
			break;
		case ID_GPS_SPEED_BP:
			td->speed = 1.0 * data / 0.0194384449;
			break;
		case ID_GPS_SPEED_AP:
			td->speed += 1.0 * data / 1.94384449; //now we are in cm/s
			td->speed = td->speed / 100 / 1000 * 3600; //now we are in km/h
			break;
		case ID_ACC_X:
			td->x = data;
			break;
		case ID_ACC_Y:
			td->y = data;
			break;
		case ID_ACC_Z:
			td->z = data;
			break;
		default:
			new_data = 0;
		break;
	}
	return new_data;
}
