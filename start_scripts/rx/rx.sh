#!/bin/bash
# rx script

#adapt these to your needs
CHANNEL="161"
NICS=`ls /sys/class/net | grep wlan`
SAVE_PATH="/media/usb0/video"
RATE="12M"

WBC_PATH="/home/pi/wifibroadcast"
DISPLAY_PROGRAM="/opt/vc/src/hello_pi/hello_video/hello_video.bin" 
OSD="/home/pi/wifibroadcast_osd/osd"
SCRIPT_PATH="/home/pi/wifibroadcast_fpv_scripts"

##################################
#change these only if you know what you are doing (and remember to change them on both sides)
BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024
PORT=0
OSD_PORT=1
##################################

function prepare_nic {
	echo "updating wifi ($1, $2)"
	ifconfig $1 down
	iw dev $1 set monitor otherbss fcsfail
	ifconfig $1 up
	iw reg set RU
	iwconfig $1 channel $2
	iwconfig $1 rate $RATE fixed
}

################################# SCRIPT START #######################


# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

#wait a bit until the wifi cards are ready
sleep 2

#start the rx in an endless loop so that is can recover in case something crashes
#prepare NICS
for NIC in $NICS
do
	prepare_nic $NIC $CHANNEL
done

$SCRIPT_PATH/start_video.sh $WBC_PATH $DISPLAY_PROGRAM $SCRIPT_PATH $BLOCK_SIZE $FECS $PACKET_LENGTH $PORT $NICS $SAVE_PATH &
$SCRIPT_PATH/start_osd.sh $WBC_PATH $OSD $SCRIPT_PATH $BLOCK_SIZE $FECS $PACKET_LENGTH $OSD_PORT $NICS &

