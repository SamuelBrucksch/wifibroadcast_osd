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
#define DRAW_ALT_LADDER true
#define DRAW_SPEED_LADDER true
#define DRAW_CURRENT false

/* #### Protocol ####
 *
 *  FRSKY -> Frsky protocoll
 *  MAVLINK -> Mavlink protocol (APM/Pixhawk/...)
 *  GPS -> direct NMEA input
 */
#define FRSKY

//Demo values used for simulations
int asc = 1;
int hor_angle = -30;
int ver_angle = -30;
double volt = 12.6d;
char buffer[50];
int heading = 0;

int getWidth(float pos_x_percent) {
	return (width / 100.0 * pos_x_percent);
}

int getHeight(float pos_y_percent) {
	return (height / 100.0 * pos_y_percent);
}

wifibroadcast_rx_status_t *status_memory_open(void) {
        int fd = shm_open("/wifibroadcast_rx_status", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

        if(fd < 0) {
                perror("shm_open");
                exit(1);
        }

        if (ftruncate(fd, sizeof(wifibroadcast_rx_status_t)) == -1) {
                perror("ftruncate");
                exit(1);
        }

        void *retval = mmap(NULL, sizeof(wifibroadcast_rx_status_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (retval == MAP_FAILED) {
                perror("mmap");
                exit(1);
        }


        return (wifibroadcast_rx_status_t*)retval;
}

wifibroadcast_rx_status_t *t;

void render_init() {
        init(&width, &height);
        mid_x = width/2;
        mid_y = height/2;
        ARROW_X = width/2-20;
        ARROW_Y = height-100;

	t = status_memory_open();
}

void render(telemetry_data_t *td) {
	Start(width, height);
	
	//paintVolt(td->voltage, VOLT_X, VOLT_Y);

	draw_altitude(td->altitude, getWidth(60), getHeight(50), DRAW_ALT_LADDER, 1.5);
	draw_speed((int)td->speed, getWidth(40), getHeight(50), DRAW_SPEED_LADDER, 1.5);
	paintArrow((int)td->heading, getWidth(50), getHeight(84));
	//draw_home_indicator((int)td->heading, getWidth(50), getHeight(50), 1.5); 
	draw_compass(td->heading, getWidth(50), getHeight(89), 2);
	draw_bat_status(td->voltage, 0.0f, getWidth(20), getHeight(5), 2);
	draw_signal(t->current_signal_dbm, getWidth(20), getHeight(90), 2);
	draw_bat_remaining(((td->voltage/CELLS)-CELL_MIN)/(CELL_MAX-CELL_MIN)*100, getWidth(10), getHeight(90), 3);

	#if defined(FRSKY)
	//we assume that if we get the NS and EW values from frsky protocol, that we have a fix
	if ((td->ew == 'E' || td->ew == 'W') && (td->ns == 'N' || td->ns == 'S')){
		//paintCoordinates((td->ns == 'N'? 1:-1) * td->latitude, (td->ew == 'E'? 1:-1) * td->longitude, width - 100, VOLT_Y + FONT_SIZE + 2);
		draw_position((td->ns == 'N'? 1:-1) * td->latitude, (td->ew == 'E'? 1:-1) * td->longitude, getWidth(5), getHeight(5), 2);
	}
	#elif defined(MAVLINK)

	#endif

	#if defined(FRSKY)
	float x_val, y_val, z_val, result;
    	x_val = td->x/1000.0f;
    	y_val = td->y/1000.0f;
    	z_val = td->z/1000.0f;

	//TODO vertical angle
	//paintAHI(180.0f / M_PI * (atan(x_val / sqrt((y_val*y_val) + (z_val*z_val)))), 180.0f/M_PI * (atan(y_val/sqrt((x_val*x_val)+(z_val*z_val)))));
	draw_horizon(180.0f / M_PI * (atan(x_val / sqrt((y_val*y_val) + (z_val*z_val)))), 180.0f/M_PI * (atan(y_val/sqrt((x_val*x_val)+(z_val*z_val)))), getWidth(50), getHeight(50),true, 1.5f);
	#elif defined(MAVLINK)

	#elif defined(LTM)

	#endif
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

void draw_signal(int8_t signal, int pos_x, int pos_y, float scale){
        sprintf(buffer, "Signal: %ddBm", signal);
        float s_width = TextWidth(buffer, SansTypeface, width / 170 * scale);
        Fill(0xff,0xff,0xff,0.5);
        StrokeWidth(0);
        Rect(pos_x-2,pos_y-2, s_width+2 , width / 170 * scale + 4);

        Fill(0,0,0,1);
        Text(pos_x, pos_y, buffer, SansTypeface, width / 170 * scale);
}

void paintArrow(int heading, int pos_x, int pos_y){
	if (heading == 360) heading = 0;
	
	//offset for arrow, so middle of the arrow is at given position
	pos_x -= 20;
	pos_y -= 20;
	
	float x[8] = {10+pos_x, 10+pos_x, 0+pos_x, 20+pos_x, 40+pos_x, 30+pos_x, 30+pos_x, 10+pos_x};
	float y[8] = {0+pos_y, 20+pos_y, 20+pos_y, 40+pos_y, 20+pos_y,20+pos_y,0+pos_y,0+pos_y};
	Fill(0xff,0xff,0xff,1);
	Stroke(0,0,0,1);
	StrokeWidth(2);

	rotatePoints(x, y, heading, 8, pos_x+20,pos_y+20);
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

//new stuff from fritz walter https://www.youtube.com/watch?v=EQ01b3aJ-rk
void draw_bat_remaining(int remaining, int pos_x, int pos_y, float scale){
	int s_width = 20 * scale;
	int s_height = 10 * scale;
	int corner = 3 * scale;
	int plus_w = 2 * scale;
	int plus_h = 5 * scale;
	int stroke = 1 * scale;

	Fill(255, 255, 255, 1);
	Stroke(0,0,0,1);
	StrokeWidth(1);
	Roundrect(pos_x - s_width / 2, pos_y - s_height / 2, s_width, s_height, corner, corner);
	Rect(pos_x + s_width / 2, pos_y - plus_h / 2, plus_w, plus_h);

	Fill(0, 0, 0, 1);
	Rect(pos_x - s_width / 2 + stroke + remaining / 100.0 * s_width, pos_y - s_height / 2 + stroke, s_width - 2 * stroke - remaining / 100.0 * s_width, s_height - 2 * stroke);
}

void draw_compass(int heading, int pos_x, int pos_y, float scale){
	int width_number = 50 * scale;
	int height_number = 20 * scale;

	VGfloat x[] = { pos_x - width_number / 2, pos_x + width_number / 2, pos_x + width_number / 2, pos_x - width_number / 2, pos_x - width_number / 2 };
	VGfloat y[] = { pos_y + height_number / 2, pos_y + height_number / 2, pos_y - height_number / 2, pos_y - height_number / 2, pos_y + height_number / 2 };

	StrokeWidth(3);
	Stroke(0, 0, 0, 1);
	Polyline(x, y, 5);
	Fill(255,255,255,0.5);
	Polygon(x, y, 5);

	Fill(0,0,0,1);
	sprintf(buffer, "%3d\xb0", heading);
	TextMid(pos_x, pos_y - height_number / 4, buffer, SansTypeface, height_number / 2);

	int width_triangle = 20 * scale;
	int space = 50 * scale;
	int p0x = pos_x;
	int p0y = pos_y + space;
	int p1x = pos_x - width_triangle / 2;
	int p1y = pos_y + space + width_triangle / 2;
	int p2x = pos_x + width_triangle / 2;
	int p2y = p1y;

	VGfloat x2[] = { p0x, p1x, p2x, p0x };
	VGfloat y2[] = { p0y, p1y, p2y, p0y };
	Stroke(0, 0, 0, 1);
	Polyline(x2, y2, 4);
	Fill(255,255,255,0.5);
	Polygon(x2, y2, 4);
	
	
	int width_comp = 150 * scale;
	int height_comp = 20 * scale;
	float ratio = width_comp / 180.0;

	int i;
	char* c;
	bool draw = false;

	for (i = (heading - 90); i <= heading + 90; i++) {
		int x = pos_x + (i - heading) * ratio;
		if (i % 15 == 0) {
			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(x, pos_y + height_comp / 3 + space / 3, x, pos_y + space / 3);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(x, pos_y + height_comp / 3 + space / 3, x, pos_y + space / 3);
		}
		if (i % 30 == 0) {
			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(x, pos_y + height_comp / 3 + space / 3 * 1.2, x, pos_y + height_comp / 3 + space / 3);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(x, pos_y + height_comp / 3 + space / 3 * 1.2, x, pos_y + height_comp / 3 + space / 3);
		}

		int j = i;
		if (j < 0)
			j = j + 360;
		if (j >= 360)
			j = j - 360;

		switch (j) {
		case 0: {
			draw = true;
			c = "N";
			break;
		}
		case 90: {
			draw = true;
			c = "W";
			break;
		}
		case 180: {
			draw = true;
			c = "S";
			break;
		}
		case 270: {
			draw = true;
			c = "E";
			break;
		}
		}
		if (draw == true) {
			Fill(0,0,0,1);
			TextMid(x, pos_y + height_comp / 3 + space / 3 * 1.6, c, SansTypeface, width / 170 * scale);
			draw = false;
		}
	}
}

void draw_bat_status(float voltage, float current, int pos_x, int pos_y, float scale){
	float space = 15 * scale;
	sprintf(buffer, "%3.2fV", voltage);
	Text(pos_x, pos_y, buffer, SansTypeface, width / 170 * scale);
	#ifdef DRAW_CURRENT
	sprintf(buffer, "%3.2fA", current);
	Text(pos_x, pos_y + space, buffer, SansTypeface, width / 170 * scale);
	#endif
}

void draw_sat(int sats, int fixtype, int pos_x, int pos_y, float scale){
	//TODO fix type
	sprintf(buffer, "S: %d F: %d", sats, fixtype);
	TextEnd(pos_x, pos_y, buffer, SansTypeface, width / 170 * scale);
}

void draw_position(float lat, float lon, int pos_x, int pos_y, float scale){
	float space = 15 * scale;
	sprintf(buffer, "Lon: %.6f", lon);
	Text(pos_x, pos_y, buffer, SansTypeface, width / 170 * scale);
	sprintf(buffer, "Lat: %.6f", lat);
	Text(pos_x, pos_y + space, buffer, SansTypeface, width / 170 * scale);
}

void draw_home_distance(int distance, int pos_x, int pos_y, float scale){
	sprintf(buffer, "%5dm", distance);
	TextMid(pos_x, pos_y, buffer, SansTypeface, width / 170 * scale);
}

//autopilot mode, mavlink specific, could be used if mode is in telemetry data of other protocols as well
void draw_mode(char *mode, int pos_x, int pos_y, float scale){
	TextEnd(pos_x, pos_y, mode, SansTypeface, width / 170 * scale);
}

void draw_home_indicator(int home_angle, int pos_x, int pos_y, float scale){
	//TODO use circle from openvg instead of this
	//center circle
	int radius = 3 * scale;
	StrokeWidth(2);
	Stroke(255, 255, 255, 1);
	float i;
	VGfloat x[100];
	VGfloat y[100];
	int z = 0;

	for (i = 0; i < 6.28; i += 0.1) {
		x[z] = sin(i) * radius + pos_x;
		y[z] = cos(i) * radius + pos_y;
		z++;
	}
	x[z] = x[0];
	y[z] = y[0];
	z++;
	Polyline(x, y, z);

	//home direction
	//todo home indication is wrong
	float length = 15 * scale;
	float arg = home_angle;
	VGfloat x1 = pos_x + sin(arg) * radius;
	VGfloat y1 = pos_y + cos(arg) * radius;
	VGfloat x2 = pos_x + sin(arg) * (radius + length);
	VGfloat y2 = pos_y + cos(arg) * (radius + length);
	Line(x1, y1, x2, y2);
}

void draw_altitude(int alt, int pos_x, int pos_y, bool ladder_enabled, float scale){
	// altitude label
	StrokeWidth(2);
	Stroke(0, 0, 0, 1);
	int space = 20 * scale;
	int s_width = 40 * scale;
	int s_height = 180 * scale;
	int label_height = 15 * scale;
	int pos_x_r = pos_x + s_width / 2 + width / 4 + space;
	int pos_y_r = pos_y;

	VGfloat x[6];
	VGfloat y[6];

	x[0] = pos_x_r;
	y[0] = pos_y_r;
	x[1] = pos_x_r + s_width / 5;
	y[1] = pos_y_r + label_height / 2;
	x[2] = x[1] + s_width;
	y[2] = y[1];
	x[3] = x[2];
	y[3] = y[2] - label_height;
	x[4] = x[1];
	y[4] = y[3];
	x[5] = x[0];
	y[5] = y[0];
	Polyline(x, y, 6);
	Fill(255,255,255,0.5);
	Polygon(x, y, 6);
	Fill(0,0,0,1);
	sprintf(buffer, "%d", alt);
	TextMid(pos_x_r + s_width / 2 + s_width / 5, pos_y - width / 170 * scale / 2, buffer, SansTypeface, width / 170 * scale);

	if (ladder_enabled){
		int width_ladder = 150 * scale;
		StrokeWidth(2);
		int width_speed = 50 * scale;
		int height_speed = 300 * scale;
		int length_bar = 5 * scale;

		int k;
		int space_text = 5 * scale;
		int distance = 100 * scale;

		//ladder altitude
		int range_altitude = 200;
		float ratio_altitude = height_speed / range_altitude;
		for (k = (int) (alt - range_altitude / 2); k <= alt + range_altitude / 2; k++) {
			int y = pos_y + (k - alt) * ratio_altitude;
			sprintf(buffer, "%d", k);
			if (k % 10 == 0) {
				int px = pos_x + width_ladder / 2 + distance + width_speed;
				int px2 = px + length_bar;
				Stroke(0,0,0,1);
				StrokeWidth(5);
				//outer black border
				Line(px, y, px2, y);
				Stroke(0xff,0xff,0xff,1);
				StrokeWidth(2);
				//inner white line
				Line(px, y, px2, y);
			}
			if (k % 100 == 0) {
				int px = pos_x + width_ladder / 2 + distance + width_speed;
				int px2 = px + 2 * length_bar;
								Stroke(0,0,0,1);
				StrokeWidth(5);
				//outer black border
				Line(px, y, px2, y);
				Stroke(0xff,0xff,0xff,1);
				StrokeWidth(2);
				//inner white line
				Line(px, y, px2, y);
				Text(pos_x + width_ladder / 2 + distance + width_speed + space_text + 2 * length_bar, y - width / 600 * scale, buffer, SansTypeface, width / 300 * scale);
			}
		}
	}
}

void draw_speed(int speed, int pos_x, int pos_y, bool ladder_enabled, float scale){
	// velocity label
	StrokeWidth(2);
	Stroke(0, 0, 0, 1);
	int space = 20 * scale;
	int s_width = 40 * scale;
	int s_height = 180 * scale;
	int label_height = 15 * scale;
	int pos_x_r = pos_x + s_width / 2 + width / 4 + space;
	int pos_y_r = pos_y;

	VGfloat x[6];
	VGfloat y[6];

	int pos_x_l = pos_x - s_width / 2 - width / 4 - space;
	int pos_y_l = pos_y;
	x[0] = pos_x_l;
	y[0] = pos_y_l;
	x[1] = pos_x_l - s_width / 5;
	y[1] = pos_y_l + label_height / 2;
	x[2] = x[1] - s_width;
	y[2] = y[1];
	x[3] = x[2];
	y[3] = y[2] - label_height;
	x[4] = x[1];
	y[4] = y[3];
	x[5] = x[0];
	y[5] = y[0];
	Polyline(x, y, 6);
	Fill(255,255,255,0.5);
	Polygon(x, y, 6);
	Fill(0,0,0,1);
	sprintf(buffer, "%d", speed);
	TextMid(pos_x_l - s_width / 2 - s_width / 5, pos_y - width / 170 * scale / 2, buffer, SansTypeface, width / 170 * scale);
	
	if (ladder_enabled){
		//ladder speed
		int width_ladder = 150 * scale;

		StrokeWidth(2);
		int width_speed = 50 * scale;
		int height_speed = 300 * scale;
		int length_bar = 5 * scale;
		int range_speed = 40;

		int k;
		int space_text = 5 * scale;
		int distance = 100 * scale;
		float ratio_speed = height_speed / range_speed;
		for (k = (int) (speed - range_speed / 2); k <= speed + range_speed / 2; k++) {
			int y = pos_y + (k - speed) * ratio_speed;
			sprintf(buffer, "%d", k);
			if (k % 1 == 0) {
				int px = pos_x - width_ladder / 2 - distance - width_speed;
				int px2 = px - length_bar;
				
				Stroke(0,0,0,1);
				StrokeWidth(5);
				//outer black border
				Line(px, y, px2, y);
				Stroke(0xff,0xff,0xff,1);
				StrokeWidth(2);
				//inner white line
				Line(px, y, px2, y);
			}
			if (k % 5 == 0) {
				int px = pos_x - width_ladder / 2 - distance - width_speed;
				int px2 = px - 2 * length_bar;
				Stroke(0,0,0,1);
				StrokeWidth(5);
				//outer black border
				Line(px, y, px2, y);
				Stroke(0xff,0xff,0xff,1);
				StrokeWidth(2);
				//inner white line
				Line(px, y, px2, y);
				if (k >= 0)
					TextEnd(pos_x - width_ladder / 2 - distance - width_speed - space_text - 2 * length_bar, y - width / 600 * scale, buffer, SansTypeface, width / 300 * scale);
			}
		}
	}
}

//ladder here means the additional lines of the AHI, if true all lines will be drawn, if false only the main line
void draw_horizon(float roll, float pitch, int pos_x, int pos_y, bool ladder_enabled, float scale){
	StrokeWidth(2);
	Stroke(255, 255, 255, 1);
	int height_ladder = 300 * scale;
	int width_ladder = 150 * scale;
	int range = 20;
	int space_text = 5 * scale;
	int pike = 4 * scale;
	float ratio = height_ladder / range;
	int k;
	Translate(pos_x, pos_y);
	Rotate(roll);
	Translate(-pos_x, -pos_y);

	int y_start = height_ladder;

	for (k = (int) (pitch - range / 2); k <= pitch + range / 2; k++){ 
		int y = pos_y + (k - pitch) * ratio;
		sprintf(buffer, "%d", k);
		if (k % 5 == 0 && k!= 0) {
			TextEnd(pos_x - width_ladder / 2 - space_text, y - width / 600 * scale, buffer, SansTypeface, width / 300 * scale);
			Text(pos_x + width_ladder / 2 + space_text, y - width / 600 * scale, buffer, SansTypeface, width / 300 * scale);
		}
		if ((k > 0) && (k % 5 == 0)) {
			int px = pos_x - width_ladder / 2;
			int px2 = pos_x + width_ladder / 2;

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px, y, px + width_ladder / 3, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px, y, px + width_ladder / 3, y);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px, y, px, y - pike);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px, y, px, y - pike);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px + width_ladder * 2 / 3, y, px2, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px + width_ladder * 2 / 3, y, px2, y);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px2, y, px2, y - pike);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px2, y, px2, y - pike);
		} else if ((k < 0) && (k % 5 == 0)) {
			int px = pos_x - width_ladder / 2 + 13 / 13.0 * width_ladder / 3;
			int px2 = pos_x - width_ladder / 2 + 10 / 13.0 * width_ladder / 3;

			int px3 = pos_x - width_ladder / 2 + 8 / 13.0 * width_ladder / 3;
			int px4 = pos_x - width_ladder / 2 + 5 / 13.0 * width_ladder / 3;

			int px5 = pos_x - width_ladder / 2 + 3 / 13.0 * width_ladder / 3;
			int px6 = pos_x - width_ladder / 2;

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px, y, px2, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px, y, px2, y);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px3, y, px4, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px3, y, px4, y);		

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px5, y, px6, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px5, y, px6, y);	

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px6, y, px6, y + pike);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px6, y, px6, y + pike);

			px = pos_x + width_ladder / 2 - 13 / 13.0 * width_ladder / 3;
			px2 = pos_x + width_ladder / 2 - 10 / 13.0 * width_ladder / 3;

			px3 = pos_x + width_ladder / 2 - 8 / 13.0 * width_ladder / 3;
			px4 = pos_x + width_ladder / 2 - 5 / 13.0 * width_ladder / 3;

			px5 = pos_x + width_ladder / 2 - 3 / 13.0 * width_ladder / 3;
			px6 = pos_x + width_ladder / 2;

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px, y, px2, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px, y, px2, y);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px3, y, px4, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px3, y, px4, y);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px5, y, px6, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px5, y, px6, y);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(px6, y, px6, y + pike);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(px6, y, px6, y + pike);

		} else if (k == 0) {
			TextEnd(pos_x - width_ladder / 1.25 - space_text, y - width / 600 * scale, buffer, SansTypeface, width / 300 * scale);
			Text(pos_x + width_ladder / 1.25 + space_text, y - width / 600 * scale, buffer, SansTypeface, width / 300 * scale);

			Stroke(0,0,0,1);
			StrokeWidth(5);
			//outer black border
			Line(pos_x - width_ladder / 1.25, y, pos_x + width_ladder / 1.25, y);
			Stroke(0xff,0xff,0xff,1);
			StrokeWidth(2);
			//inner white line
			Line(pos_x - width_ladder / 1.25, y, pos_x + width_ladder / 1.25, y);
		}
	}
}
