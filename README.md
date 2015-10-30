# wifibroadcast_osd
OSD for HD wireless FPV system based on wifibroadcast from befi

This project uses the openvg library to draw 2d objects onto the screen. It is an OSD that uses the telemetry of already existing systems like mavlink, frsky direct GPS and so on.

#Installation
1. increase split memory for GPU in case it is not already 128MB

```
sudo raspi-config
```
In advanced -> Memory Split -> 128MB
```

2. install requirements for openvg
```
sudo apt-get install libjpeg8-dev indent libfreetype6-dev ttf-dejavu-core
```

3. download modified openvg library (uses layer 1 instead of 0 and the default background is transparent)
```
git clone https://github.com/myshopradio/openvg.git
```

4. Install library
```
cd openvg
make library
sudo make install
```

After that go into the wifibroadcast_osd folder and run
```
make
./osd
```
