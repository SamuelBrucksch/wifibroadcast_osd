# wifibroadcast_osd
OSD for HD wireless FPV system based on wifibroadcast from befi

This project uses the openvg library to draw 2d objects onto the screen. It is an OSD that uses the telemetry of already existing systems like mavlink, frsky direct GPS and so on.

# Possible issues
1) The AHI might indicate the opposite direction. This can be fixed by changing this part in render.c
```
paintAHI(180.0f / M_PI * (atan(x_val / sqrt((y_val*y_val) + (z_val*z_val)))), 180.0f/M_PI * (atan(y_val/sqrt((x_val*x_val)+(z_val*z_val)))));
```
If you want to change the direction change 180.0f to -180.0f. First part is horizontal indication, second part is vertical indication.

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
It is possible to change the size of the Text. Open render.c, in line 17 there you will find #define FONT_SIZE 40, change that value to whatever you like and rebuild (make clean && make).

Later all configuration values will be in a seperate file for easier configuration. Maybe it will be even possible to modify this during runtime.
