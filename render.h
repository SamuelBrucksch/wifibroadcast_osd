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

void render_init();
void render(telemetry_data_t *td);

//rotate a polyline/polygon with this
void rotatePoints(float *x, float *y, int angle, int points, int center_x, int center_y);

void paintArrow(int heading, int pos_x, int pos_y);
void paintVolt(double volt, int pos_x, int pos_y);
void paintAHI(int hor_angle, int ver_angle);
void paintAlt(int alt, int pos_x, int pos_y);
void paintHomeDist(int dist, int pos_x, int pos_y);
void paintCoordinates(float lat, float lon, int pos_x, int pos_y);
void paintSpeed(int speed, int pos_x, int pos_y);
void paintCourse(int course, int pos_x, int pos_y);
