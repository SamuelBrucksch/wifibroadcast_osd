#pragma once

#include "bcm_host.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"
#include <math.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <stdio.h>
#include "telemetry.h"
#include "../wifibroadcast/lib.h"

#include <errno.h>
#include <resolv.h>
#include <string.h>
#include <utime.h>
#include <unistd.h>
#include <getopt.h>
#include <pcap.h>
#include <endian.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>


void render_init();
void render(telemetry_data_t *td);

//rotate a polyline/polygon with this
void rotatePoints(float *x, float *y, int angle, int points, int center_x, int center_y);

void paintArrow(int heading, int pos_x, int pos_y);
void paintVolt(double volt, int pos_x, int pos_y);
void paintAHI(int hor_angle, int ver_angle);
void paintAlt(int alt, int pos_x, int pos_y);
void paintHomeDist(int dist, int pos_x, int pos_y);
void paintCoordinates(double lat, double lon, int pos_x, int pos_y);
void paintSpeed(int speed, int pos_x, int pos_y);
void paintCourse(int course, int pos_x, int pos_y);

void draw_signal(int8_t signal, int package_rssi, int pos_x, int pos_y, float scale);

//new stuff from fritz walter https://www.youtube.com/watch?v=EQ01b3aJ-rk
//this will only indicate how much % are left. Mavlink specific, but could be used with others as well.
void draw_bat_remaining(int remaining, int pos_x, int pos_y, float scale);
void draw_compass(int heading, int pos_x, int pos_y, float scale);
void draw_bat_status(float voltage, float current, int pos_x, int pos_y, float scale);
void draw_sat(int sats, int fixtype, int pos_x, int pos_y, float scale);
void draw_position(float lat, float lon, int pos_x, int pos_y, float scale);
void draw_home_distance(int distance, int pos_x, int pos_y, float scale);
//autopilot mode, mavlink specific, could be used if mode is in telemetry data of other protocols as well
void draw_mode(char *mode, int pos_x, int pos_y, float scale);
void draw_home_indicator(int home_angle, int pos_x, int pos_y, float scale);
void draw_altitude(int alt, int pos_x, int pos_y, bool ladder_enabled, float scale);
void draw_speed(int speed, int pos_x, int pos_y, bool ladder_enabled, float scale);
//ladder here means the additional lines of the AHI, if true all lines will be drawn, if false only the main line
void draw_horizon(float roll, float pitch, int pos_x, int pos_y, bool ladder_enabled, float scale);
