#pragma once

typedef struct {
	float voltage;
	float altitude;
	float baro_altitude;
	double longitude;
	double latitude;
	float heading;
	float speed;
	int16_t x, y, z;
	int16_t ew, ns;
} telemetry_data_t;

