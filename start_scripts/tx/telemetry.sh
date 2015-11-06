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


#start the tx in an endless loop so that is can recover in case something crashes
while :
do
	echo "Starting tx on $NIC for telemetry"
        cat /dev/ttyAMA0 | $WBC_PATH/tx -p $OSD_PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NIC

        killall cat
        ps -ef | grep "tx -p $OSD_PORT" | grep -v grep | awk '{print $2}' | xargs kill -9
	sleep 1
done
