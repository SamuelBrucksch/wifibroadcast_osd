while :
do
	if [ -d "$9" ]; then
		echo "Starting with recording"
		FILE_NAME=$9/`ls $9 | wc -l`.rawvid
		$1/rx -p $7 -b $4 -r $5 -f $6 $8 | tee $FILE_NAME | $2
	else
		echo "Starting without recording"
		$1/rx -p $7 -b $4 -r $5 -f $6 $8 | $2
	fi

        killall hello_video.bin
	ps -ef | grep "rx -p $7" | grep -v grep | awk '{print $2}' | xargs kill -9
	sleep 1
done
