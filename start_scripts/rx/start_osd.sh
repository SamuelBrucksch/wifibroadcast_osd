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
	$WBC_PATH/rx -p $OSD_PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NICS | $OSD

        killall osd
	ps -ef | grep "rx -p $OSD_PORT" | grep -v grep | awk '{print $2}' | xargs kill -9
	sleep 1
done
