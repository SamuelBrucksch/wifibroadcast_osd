#pragma once

#include <stdint.h>
#include <time.h>


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
	float baro_altitude;
	float altitude;
	double longitude;
	double latitude;
	float heading;
	float speed;
	int16_t x, y, z;
	int16_t ew, ns;
	wifibroadcast_rx_status_t *rx_status;	
} telemetry_data_t;


wifibroadcast_rx_status_t *telemetry_wbc_status_memory_open(void);


