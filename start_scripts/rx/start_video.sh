NICS=`ls /sys/class/net | grep wlan`
SAVE_PATH="/media/usb0/video"
WBC_PATH="/home/pi/wifibroadcast"
DISPLAY_PROGRAM="/opt/vc/src/hello_pi/hello_video/hello_video.bin" 
OSD="/home/pi/wifibroadcast_osd/osd"
SCRIPT_PATH="/home/pi/wifibroadcast_fpv_scripts"
BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024
PORT=0
OSD_PORT=1


while :
do
	if [ -d "$SAVE_PATH" ]; then
		echo "Starting with recording"
		FILE_NAME=$SAVE_PATH/`ls $SAVE_PATH | wc -l`.rawvid
		$WBC_PATH/rx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NICS | tee $FILE_NAME | $DISPLAY_PROGRAM
	else
		echo "Starting without recording"
		$WBC_PATH/rx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NICS | $DISPLAY_PROGRAM
	fi

        killall hello_video.bin
	ps -ef | grep "rx -p $PORT" | grep -v grep | awk '{print $2}' | xargs kill -9
	sleep 1
done
