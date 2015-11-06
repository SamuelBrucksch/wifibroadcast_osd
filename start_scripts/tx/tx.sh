#!/bin/bash
# tx script

#adapt these to your needs
NIC="wlan0"
CHANNEL="161"
RATE="12M"

WIDTH=1280
HEIGHT=720
FPS=48
BITRATE=4000000
KEYFRAMERATE=8

##################################

#change these only if you know what you are doing (and remember to change them on both sides)
BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024
PORT=0
OSD_PORT=1
##################################

WBC_PATH="/home/pi/wifibroadcast"
SCRIPT_PATH="/home/pi/wifibroadcast_fpv_scripts"

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

#wait a bit. this help automatic starting
sleep 2

echo "updating wifi ($NIC, $CHANNEL)"
ifconfig $NIC down
iw dev $NIC set monitor otherbss fcsfail
ifconfig $NIC up
iw reg set RU
iwconfig $NIC channel $CHANNEL
iwconfig $NIC rate $RATE fixed

stty -F /dev/ttyAMA0 -imaxbel -opost -isig -icanon -echo -echoe -ixoff -ixon 9600 #set up serial port
$SCRIPT_PATH/telemetry.sh &
$SCRIPT_PATH/raspivid.sh &
