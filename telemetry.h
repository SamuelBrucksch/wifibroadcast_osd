#pragma once

typedef struct {
	float voltage;
	float altitude;
	double longitude;
	double latitude;
	float heading;
	float speed;
	int16_t x, y, z;
} telemetry_data_t;

