/** BATTERY **/
//TODO automatically calculate cells based on voltage
#define CELLS 3
#define CELL_MAX 4.20
#define CELL_MIN 3.30
//warning level 1 -> orange
#define CELL_WARNING1 3.80
//warning level 2 -> red, critical
#define CELL_WARNING2 3.60

/** ARTIFICIAL HORIZON **/
//set to 1 or -1
#define INVERT_ROLL 1
#define INVERT_PITCH 1
//uncomment if roll and pitch are exchanged
//#define EXCHANGE_ROLL_AND_PITCH

/** HOME ARROW **/
//uncomment if home arrow is pointing in the wrong direction
//#define INVERT_HOME_ARROW

/** UNITS**/
//uncomment if you want to use imperial units
//#define IMPERIAL

/** DETAILS **/
//set to true if you want to show the ladders
//on raspi 1 it is not recommended to use the ladders as it might reduce performance and cause artefacts in the video due too much load
#define DRAW_ALT_LADDER false
#define DRAW_SPEED_LADDER false
#define DRAW_COURSE_LADDER true
#define DRAW_AHI_LADDER true

/** OSD ELEMENTS **/
//comment to disable feature
#define RSSI
#define HEADING
#define HOME_ARROW
#define BATT_REMAINING
#define BATT_STATUS
#define ALT
#define SPEED
#define POSITION
#define HORIZON
#define DISTANCE
//#define DRAW_CURRENT

/** PROTOCOL **/
/* FRSKY -> Frsky protocoll
 * MAVLINK -> Mavlink protocol (APM/Pixhawk/...)
 * GPS -> direct NMEA input
 * LTM -> Light Telemetry
 *
 * Only frsky is currently working
 */
//#define FRSKY
#define LTM

/** RSSI **/
//packet based rssi, uncomment to disable
//TODO not implemented yet
//#define PACKET_BASED_RSSI
