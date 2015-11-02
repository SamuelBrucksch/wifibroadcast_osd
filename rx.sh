#!/bin/bash
# rx script

#adapt these to your needs
CHANNEL="161"
NICS=`ls /sys/class/net | grep wlan`
SAVE_PATH="/media/usb0/video"

WBC_PATH="/home/pi/wifibroadcast"
OSD_PATH="/home/pi/wifibroadcast_osd"
DISPLAY_PROGRAM="/opt/vc/src/hello_pi/hello_video/hello_video.bin" 

##################################
#change these only if you know what you are doing (and remember to change them on both sides)
BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024
PORT=0
TELEM_PORT=1
##################################

function prepare_nic {
	echo "updating wifi ($1, $2)"
	killall ifplugd
	ifconfig $1 down
	iw dev $1 set monitor otherbss fcsfail
	ifconfig $1 up
	iw reg set RU
	iwconfig $1 channel $2
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
while :
do
	#prepare NICS
	for NIC in $NICS
	do
		prepare_nic $NIC $CHANNEL
	done

	if [ -d "$SAVE_PATH" ]; then
		echo "Starting OSD"
                $WBC_PATH/rx -p $TELEM_PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NICS | $OSD_PATH/osd &

		echo "Starting with recording"
		FILE_NAME=$SAVE_PATH/`ls $SAVE_PATH | wc -l`.rawvid
		$WBC_PATH/rx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NICS | tee $FILE_NAME | $DISPLAY_PROGRAM
	else
		echo "Starting OSD"
                $WBC_PATH/rx -p $TELEM_PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NICS | $OSD_PATH/osd &

		echo "Starting without recording"
		$WBC_PATH/rx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NICS | $DISPLAY_PROGRAM
 	fi

	sleep 1
done
