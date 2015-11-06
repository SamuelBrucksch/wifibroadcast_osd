while :
do
	$1/rx -p $7 -b $4 -r $5 -f $6 $8 | $2

        killall osd
	ps -ef | grep "rx -p $7" | grep -v grep | awk '{print $2}' | xargs kill -9
	sleep 1
done
