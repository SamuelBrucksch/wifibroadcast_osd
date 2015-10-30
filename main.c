#include "osd.h"

//display size
int width, height;

#define CELLS 3
#define CELL_MAX 4.20
#define CELL_MIN 3.00
#define DRAW_FPS 25

//positioning of objects, could be exchanged with configurable interface later
int ARROW_X = 0;
int ARROW_Y = 0;
#define VOLT_X 90
#define VOLT_Y 50
int mid_y_ahi, mid_x;

//Demo values used for simulations
int asc = 1;
int hor_angle = -30;
int ver_angle = -30;
double volt = 12.6d;
char buffer[50];
int heading = 0;

int main() {
	// Graphics initialization
	init(&width, &height);

	mid_y_ahi = height/2;
	mid_x = width/2;

	//TODO configurable position of drawn objects
	ARROW_X = width/2 -20;
	ARROW_Y = height -100;
 
	srand(time(NULL));
	float x =0;
	float y = 0;

	float theta = 0;
	float h = 12;
	float k = 10;
	float step = 5;
	float r = 150;

	while (1){
		if (theta < 360){
			x = h + r*cos(theta*0.017453292519);
			y = k + r*sin(theta*0.017453292519);
			theta +=step;
		}else{
			theta = 0;
		}
		Start(width, height);

		#ifdef PAINT_TRACK
			paintTrackWindow();
			addPointToTrack(x,y);
		#endif

		paintVolt(volt, VOLT_X, VOLT_Y);
		paintAlt(100, VOLT_X, height/2);
		paintSpeed(45, width - VOLT_X, height/2);
		paintAHI(hor_angle, ver_angle);
		paintHomeDist(500, width/2, VOLT_Y);
		
		//TODO 0,0 is just a placeholder for configurable position later
		paintArrow(heading++, 0, 0);
		End();
    
    		
    		if (asc){
			hor_angle++;
			ver_angle++;
		}else{
			hor_angle--;
			ver_angle--;
		}
		volt -= 0.01;
	
		if (hor_angle == 30){
			asc=0;
		}else if (hor_angle == -30){
			asc = 1;
		}
		usleep(1000/DRAW_FPS*1000);
	}
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
	
	float width = TextWidth(buffer, SansTypeface, 28);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2,pos_y-2, width+2 , 32);
	
	if (volt/CELLS > CELL_MAX-((CELL_MAX-CELL_MIN)/3)){
		Fill(0x0,0xff,0x0,1);
	}else if (volt/CELLS > CELL_MAX-((CELL_MAX-CELL_MIN)/3*2)){
		Fill(255,165,0x0,1);
	}else{
		Fill(255, 0,0,1);
	}
	Text(pos_x, pos_y, buffer, SansTypeface, 28);
}

void paintAHI(int hor_angle, int ver_angle){
	//TODO calculate correct value based on horizontal angle
	mid_y_ahi += ver_angle;
	
	int offset_x = (width/6*cos(hor_angle*0.017453292519));
	int offset_y = (width/6*sin(hor_angle*0.017453292519));
	
	Stroke(0,0,0,1);
	StrokeWidth(5);
	//outer black border
	Line(mid_x - offset_x,mid_y_ahi - offset_y, mid_x + offset_x, mid_y_ahi + offset_y);
	Stroke(0xff,0xff,0xff,1);
	StrokeWidth(2);
	//inner white line
	Line(mid_x - offset_x,mid_y_ahi - offset_y, mid_x + offset_x, mid_y_ahi + offset_y);
}

void paintAlt(int alt, int pos_x, int pos_y){
	//TODO font stroke
	sprintf(buffer, "Alt: %dm", alt);
	
	float width = TextWidth(buffer, SansTypeface, 28);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2,pos_y-2, width+2 , 32);

	Fill(0,0,0,1);
	Text(pos_x, pos_y, buffer, SansTypeface, 28);
}

void paintHomeDist(int dist, int pos_x, int pos_y){
	//TODO font stroke
	sprintf(buffer, "Dist: %dm", dist);
	
	float width = TextWidth(buffer, SansTypeface, 28);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2 - 0.5*width,pos_y-2, width+2 , 32);

	Fill(0,0,0,1);
	Text(pos_x-0.5*width, pos_y, buffer, SansTypeface, 28);
}

void paintCoordinates(float lat, float lon, int pos_x, int pos_y){

}

void paintSpeed(int speed, int pos_x, int pos_y){
	//TODO font stroke
	sprintf(buffer, "Speed: %dkm/h", speed);
	
	float width = TextWidth(buffer, SansTypeface, 28);
	Fill(0xff,0xff,0xff,0.5);
	StrokeWidth(0);
	Rect(pos_x-2-width,pos_y-2, width+2 , 32);

	Fill(0,0,0,1);
	Text(pos_x-width, pos_y, buffer, SansTypeface, 28);
}

void paintCourse(int course, int pos_x, int pos_y){

}

#if defined PAINT_TRACK
float lenArray[TRACK_POINTS];
float angleArray[TRACK_POINTS];
void paintTrackWindow(){
	Fill(0xff,0xff,0xff,0.5);
	Stroke(0,0,0,1);
	StrokeWidth(1);
	Rect(width - 350 + 1, 1, 350, 350);

	//TODO draw lines 
	Polyline(lenArray , angleArray, TRACK_POINTS);
}

void addPointToTrack(float len, float angle){
	int i = TRACK_POINTS-1;
	for (i; i > 0; i--){
		lenArray[i] = lenArray[i-1];
		angleArray[i] = angleArray[i-1];
	} 
	lenArray[0] = len + width - 352/2;
	angleArray[0] = angle + 352/2;
}
#endif