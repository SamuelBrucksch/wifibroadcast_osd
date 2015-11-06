#adapt these to your needs
NIC="wlan0"
CHANNEL="161"
RATE="12M"


WIDTH=1280
HEIGHT=800
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
        echo "Starting tx for $NIC for raspivid"
        raspivid -ih -t 0 -ex sports -awb horizon -ss 7000 -w $WIDTH -h $HEIGHT -fps $FPS -b $BITRATE -n -g $KEYFRAMERATE -pf high -o - | $WBC_PATH/tx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NIC

        killall raspivid
	ps -ef | grep "tx -p $PORT" | grep -v grep | awk '{print $2}' | xargs kill -9
        sleep 1
done

