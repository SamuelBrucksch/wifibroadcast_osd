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

//rotate a polyline/polygon with this
void rotatePoints(float *x, float *y, int angle, int points, int center_x, int center_y);

void paintArrow(int heading);

//draw voltage text at given position
void paintVolt(double volt, int pos_x, int pos_y);

void paintAHI(int hor_angle, int ver_angle);