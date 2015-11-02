#include <stdint.h>
#include "render.h"
#include "telemetry.h"

int width, height;
const float MIN_VOLTAGE = 10.5;
#define CELLS 3
#define CELL_MAX 4.20
#define CELL_MIN 3.00

//positioning of objects, could be exchanged with configurable interface later
int ARROW_X = 0;
int ARROW_Y = 0;
#define VOLT_X 90
#define VOLT_Y 50
int mid_y, mid_x;
#define FONT_SIZE 40

//Demo values used for simulations
int asc = 1;
int hor_angle = -30;
int ver_angle = -30;
double volt = 12.6d;
char buffer[50];
int heading = 0;

void render_init() {
	init(&width, &height);
	mid_x = width/2;
	mid_y = height/2;
	ARROW_X = width/2-20;
	ARROW_Y = height-100;
}

void render(telemetry_data_t *td) {
	Start(width, height);

	paintVolt(td->voltage, VOLT_X, VOLT_Y);
	paintAlt((int)td->altitude, VOLT_X, height/2);
	paintSpeed((int)td->speed, width - VOLT_X, height/2);

	float x_val, y_val, z_val, result;
    	x_val = td->x/1000.0f;
    	y_val = td->y/1000.0f;
    	z_val = td->z/1000.0f;

	//TODO vertical angle
	paintAHI(180.0f / M_PI * (atan(x_val / sqrt((y_val*y_val) + (z_val*z_val)))), 180.0f/M_PI * (atan(y_val/sqrt((x_val*x_val)+(z_val*z_val)))));

	//TODO calculate distance to home
	//paintHomeDist(500, width/2, VOLT_Y);
	
	//we assume that if we get the NS and EW values from frsky protocol, that we have a fix
	if ((td->ew == 'E' || td->ew == 'W') && (td->ns == 'N' || td->ns == 'S')){
		paintCoordinates((td->ns == 'N'? 1:-1) * td->latitude, (td->ew == 'E'? 1:-1) * td->longitude, width - 100, VOLT_Y + FONT_SIZE + 2);
	}


	//TODO 0,0 is just a placeholder for configurable position later
	//TODO exchange heading with home arrow
	paintArrow((int)td->heading, 0, 0);
	End();
}

void rotatePoints(float *x, float *y, int angle, int points, int center_x, int center_y){
	double cosAngle = cos(-angle  * M_PI / 180.0f);
	double sinAngle = sin(-angle  * M_PI / 180.0f);

	int i = 0;
	int tmp_x = 0;
	int tmp_y = 0;
	for (i=0; i < points; i++){
		tmp_x = center_x + (x[i]-center_x)*cosAngle-(y[i]-center_y)*sinAngle;
		tmp_y = center_y + (x[i]-center_x)*sinAngle + (y[i] - center_y)*cosAngle;
		x[i] = tmp_x;
		y[i] = tmp_y;
	}
}

void paintArrow(int heading, int pos_x, int pos_y){
	if (heading == 360) heading = 0;
	float x[8] = {10+ARROW_X, 10+ARROW_X, 0+ARROW_X, 20+ARROW_X, 40+ARROW_X, 30+ARROW_X, 30+ARROW_X, 10+ARROW_X};
	float y[8] = {0+ARROW_Y, 20+ARROW_Y, 20+ARROW_Y, 40+ARROW_Y, 20+ARROW_Y,20+ARROW_Y,0+ARROW_Y,0+ARROW_Y};
	Fill(0xff,0xff,0xff,1);
	Stroke(0,0,0,1);
	StrokeWidth(2);

	rotatePoints(x, y, heading, 8, ARROW_X+20,ARROW_Y+20);
	Polygon(x, y, 8);
	Polyline(x, y, 8);
}

void paintVolt(double volt, int pos_x, int pos_y){
	//TODO font stroke
	sprintf(buffer, "%0.2fV", volt);

	float width = TextWidth(buffer, SansTypeface, FONT_SIZE);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2,pos_y-2, width+2 , FONT_SIZE+4);

	if (volt/CELLS > CELL_MAX-((CELL_MAX-CELL_MIN)/3)){
		Fill(0x0,0xff,0x0,1);
	}else if (volt/CELLS > CELL_MAX-((CELL_MAX-CELL_MIN)/3*2)){
		Fill(255,165,0x0,1);
	}else{
		Fill(255, 0,0,1);
	}
	Text(pos_x, pos_y, buffer, SansTypeface, FONT_SIZE);
}

void paintAHI(int hor_angle, int ver_angle){
	//if vertical angle is larger than 45° leave the ahi at highest pos
	if (ver_angle > 45)
		ver_angle = 45;
	else if (ver_angle < -45)
		ver_angle = -45;

	int offset_x = (width/6*cos(hor_angle*0.017453292519));
	int offset_y = (width/6*sin(hor_angle*0.017453292519));

	int horizon_y = (mid_y - 100)/45*ver_angle + mid_y;

	Stroke(0,0,0,1);
	StrokeWidth(5);
	//outer black border
	Line(mid_x - offset_x,horizon_y - offset_y, mid_x + offset_x, horizon_y + offset_y);
	Stroke(0xff,0xff,0xff,1);
	StrokeWidth(2);
	//inner white line
	Line(mid_x - offset_x,horizon_y - offset_y, mid_x + offset_x, horizon_y + offset_y);
}

void paintAlt(int alt, int pos_x, int pos_y){
	//TODO font stroke
	sprintf(buffer, "Alt: %dm", alt);

	float width = TextWidth(buffer, SansTypeface, FONT_SIZE);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2,pos_y-2, width+2 , FONT_SIZE + 4);

	Fill(0,0,0,1);
	Text(pos_x, pos_y, buffer, SansTypeface, FONT_SIZE);
}

void paintHomeDist(int dist, int pos_x, int pos_y){
	//TODO font stroke
	sprintf(buffer, "Dist: %dm", dist);

	float width = TextWidth(buffer, SansTypeface, FONT_SIZE);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2 - 0.5*width,pos_y-2, width+2 , FONT_SIZE + 4);

	Fill(0,0,0,1);
	Text(pos_x-0.5*width, pos_y, buffer, SansTypeface, FONT_SIZE);
}

void paintCoordinates(double lat, double lon, int pos_x, int pos_y){
	sprintf(buffer, "%0.6f", lat);
	float width = TextWidth(buffer, SansTypeface, FONT_SIZE);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-width,pos_y-2, width+2 , FONT_SIZE + 4);
	Fill(0,0,0,1);
	TextEnd(pos_x, pos_y , buffer, SansTypeface, FONT_SIZE);

	sprintf(buffer, "%0.6f", lon);
	width = TextWidth(buffer, SansTypeface, FONT_SIZE);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-width,pos_y-2 - FONT_SIZE - 4, width+2 , FONT_SIZE + 4);
	Fill(0,0,0,1);
	TextEnd(pos_x, pos_y - FONT_SIZE - 4, buffer, SansTypeface, FONT_SIZE);
}

void paintSpeed(int speed, int pos_x, int pos_y){
	//TODO font stroke
	sprintf(buffer, "Speed: %dkm/h", speed);

	float width = TextWidth(buffer, SansTypeface, FONT_SIZE);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2-width,pos_y-2, width+2 , FONT_SIZE + 4);

	Fill(0,0,0,1);
	Text(pos_x-width, pos_y, buffer, SansTypeface, FONT_SIZE);
}

void paintCourse(int course, int pos_x, int pos_y){

}
