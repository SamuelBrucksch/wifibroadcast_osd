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

//paints the last 30 coordinates 
//if not wanted uncomment
#define PAINT_TRACK
#define TRACK_POINTS 50

//rotate a polyline/polygon with this
void rotatePoints(float *x, float *y, int angle, int points, int center_x, int center_y);

void paintArrow(int heading, int pos_x, int pos_y);

//draw voltage text at given position
void paintVolt(double volt, int pos_x, int pos_y);

void paintAHI(int hor_angle, int ver_angle);

void paintAlt(int alt, int pos_x, int pos_y);
void paintHomeDist(int dist, int pos_x, int pos_y);
void paintCoordinates(float lat, float lon, int pos_x, int pos_y);
void paintSpeed(int speed, int pos_x, int pos_y);
void paintCourse(int course, int pos_x, int pos_y);

#if defined PAINT_TRACK
void paintTrackWindow();
void addPointToTrack(float len, float angle);
#endif