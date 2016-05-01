#!/bin/bash


#if we detect the camera, we fall asleep
if vcgencmd get_camera | grep -q detected=1; then
	echo "osd.sh: Falling asleep because a camera has been detected"
	sleep 365d
fi

#adapt these to your needs
NIC="wlan0"
PORT=1
SAVE_PATH="/media/usb0/telemetry"

BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024

WBC_PATH="/home/pi/wifibroadcast"
FRSKY_OMX_OSD_PATH="/home/pi/wifibroadcast_osd/osd"

sleep 10

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi


if [ -d "$SAVE_PATH" ]; then
	echo "Starting osd with recording"
	$WBC_PATH/rx -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH -p $PORT $NIC | tee $SAVE_PATH/`ls $SAVE_PATH | wc -l`.frsky | $FRSKY_OMX_OSD_PATH
else
	echo "Starting osd without recording"
	#$WBC_PATH/rx -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH -p $PORT $NIC
	$WBC_PATH/rx -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH -p $PORT $NIC | $FRSKY_OMX_OSD_PATH

fi


