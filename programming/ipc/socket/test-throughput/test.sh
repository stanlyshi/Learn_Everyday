#!/bin/bash
# 进行一系列的测试 rongtao@sylincom.com

mapfile=test.txt

function execute() {
for ((isize=100;isize<3000;isize+=100))
do 
for ((inum=200;inum<100000;inum+=200))
do 
	mbps=`./throughput --dst-addr 10.170.7.166 \
				--port 2152 \
				--peer-type client \
				--proto-type tcp \
				--pkg-size $isize \
				--pkg-num $inum \
				--read-reply | grep Client | awk '{print $5}' | sed '$!N;s/\n/ \t\t /'`

	echo -e "$isize \t\t $inum \t\t $mbps " >> $mapfile
	usleep 100
done
done
}

rm -f $mapfile
execute
