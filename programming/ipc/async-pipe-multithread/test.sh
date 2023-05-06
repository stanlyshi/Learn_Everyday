#!/bin/bash
# rongtao@sylincom.com

# ./main [nthread] [max-pkg-size] [pkg-size-interval] [record-file-name]

max_pkg_size=10000
pkg_size_interval=25


for it in  1 2 3 4 5
do
	./main $it $max_pkg_size $pkg_size_interval record-$it-$max_pkg_size-$pkg_size_interval.txt
done
