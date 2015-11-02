#!/bin/bash
# tx script

#adapt these to your needs
NIC="wlan0"
CHANNEL="161"

WIDTH=1280
HEIGHT=720
FPS=48
BITRATE=4000000
KEYFRAMERATE=48

##################################

#change these only if you know what you are doing (and remember to change them on both sides)
BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024
PORT=0

##################################

WBC_PATH="/home/pi/wifibroadcast"

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

#wait a bit. this help automatic starting
sleep 2

#start the tx in an endless loop so that is can recover in case something crashes
while :
do
	echo "updating wifi ($NIC, $CHANNEL)"
	killall ifplugd
	ifconfig $NIC down
	iw dev $NIC set monitor otherbss fcsfail
	ifconfig $NIC up
	iwconfig $NIC channel $CHANNEL

	echo "Starting tx for $NIC"
#	raspivid -ih -t 0 -w $WIDTH -h $HEIGHT -fps $FPS -b $BITRATE -n -g $KEYFRAMERATE -pf high -o - | $WBC_PATH/tx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NIC
	$WBC_PATH/tx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH -s 2 $NIC &
	stty -F /dev/ttyAMA0 -imaxbel -opost -isig -icanon -echo -echoe -ixoff -ixon -9600 # setting up serial port
	raspivid -ih -t 0 -w $WIDTH -h $HEIGHT -fps $FPS -b $BITRATE -n -g $KEYFRAMERATE -pf high -o - > /tmp/fifo0 &
	#sleep a bit so raspivid can initialize	
	sleep 10
	cat /dev/ttyAMA0 > /tmp/fifo1

	killall raspivid
	killall tx
	sleep 1
done
