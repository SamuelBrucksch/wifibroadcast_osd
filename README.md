# wifibroadcast_osd
OSD for HD wireless FPV system based on wifibroadcast from befi

This project uses the openvg library to draw 2d objects onto the screen. It is an OSD that uses the telemetry of already existing systems like mavlink, frsky direct GPS and so on.

If some steps in setting up the osd is unclear visit the blog from befi and check if it helps:
https://befinitiv.wordpress.com/2015/07/06/telemetry-osd-for-wifibroadcast/

Most of the steps should be the same.

# Possible issues
0) Current implementation is pretty slow. It is recommended to disable the speed and alt ladder, this can be done with the #define ALT_LADDER and SPEED_LADDER in the upper part of render.c Just change true to false. This will already improve the speed a lot. Also it is recommended to currently use paintAHI instead of draw_horizon.

1) The AHI might indicate the opposite direction. This can be fixed by changing this part in render.c
```
//set to 1 or -1
#define INVERT_ROLL 1
#define INVERT_PITCH 1
```
-1 for reversed and 1 for normal.

2) The home arrow currently does not point to home, but shows the heading (like a compass).

3) Distance does not work yet as well as everything related to coordinates

#Installation
1) increase split memory for GPU in case it is not already 128MB

```
sudo raspi-config
```
In advanced -> Memory Split -> 128MB


2) install requirements for openvg
```
sudo apt-get install libjpeg8-dev indent libfreetype6-dev ttf-dejavu-core
```

3) download modified openvg library (uses layer 1 instead of 0 and the default background is transparent)
```
cd
git clone https://github.com/myshopradio/openvg.git
```

4) Install library
```
cd openvg
make library
sudo make install
```

5) Download and compile osd source code
```
cd
git clone https://github.com/SamuelBrucksch/wifibroadcast_osd.git
cd wifibroadcast_osd
make
```

#Configuration
Later all configuration values will be in a seperate file for easier configuration. Maybe it will be even possible to modify this during runtime. But right now everything is in testing mode only, so no config yet.

# Starting OSD
I uploaded my start scripts as a sample how to start wifibroadcast with 1 video and 1 telemetry stream. have a look at those and adapt your own start scripts based on that.

Telemetry serial on TX can directly be connected to the Raspi serial interface. You need to disable the Linux Serial Console for this. Start raspi-config and go advanced -> serial and disable. This needs to be done on TX and on RX.

The Raspi Serial expects TTL 3.3V Level, so do not use with any telemetry source that uses 5V TTL Level or even RS232.
