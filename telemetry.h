#pragma once

#include <stdint.h>
#include <time.h>
#include "osdconfig.h"

typedef struct {
        uint32_t received_packet_cnt;
        uint32_t wrong_crc_cnt;
        int8_t current_signal_dbm;
} wifi_adapter_rx_status_t;

typedef struct {
        time_t last_update;
        uint32_t received_block_cnt;
        uint32_t damaged_block_cnt;
        uint32_t tx_restart_cnt;

        uint32_t wifi_adapter_cnt;
        wifi_adapter_rx_status_t adapter[8];
} wifibroadcast_rx_status_t;

typedef struct {
	float voltage;
	float ampere;
	float baro_altitude;
	float altitude;
	double longitude;
	double latitude;
	float heading;
	float speed;
	int16_t x, y, z;
	int16_t ew, ns;
#if defined(LTM) || defined(MAVLINK)
	int16_t roll, pitch;
	uint8_t rssi;
	uint8_t airspeed;
	uint8_t sats;
	uint8_t fix;
#endif
	wifibroadcast_rx_status_t *rx_status;	
} telemetry_data_t;

wifibroadcast_rx_status_t *telemetry_wbc_status_memory_open(void);
