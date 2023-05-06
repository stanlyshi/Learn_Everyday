#!/bin/bash
# rongtao@sylincom.com

default_serveraddr=10.170.6.66
default_protocol=tcp
default_port=2152
default_pkg_size=1500
default_pkg_num=1000

#报文有来有回
function server_dupkg() {
./throughput --port $default_port \
		--peer-type server \
		--proto-type $1 \
		--reply \
		--test-malloc
}

#报文有来有回
function client_dupkg() {
./throughput --dst-addr $2 \
		--port $default_port \
		--peer-type client \
		--proto-type $1 \
		--pkg-size $3 \
		--pkg-num $4 \
		--read-reply \
		--test-malloc 
}

#报文有来无回
function server_original() {
./throughput --port $default_port \
		--peer-type server \
		--proto-type $1 \
		--test-malloc
}

#报文有来无回
function client_original() {
./throughput --dst-addr $2 \
		--port $default_port \
		--peer-type client \
		--proto-type $1 \
		--pkg-size $3 \
		--pkg-num $4 \
		--test-malloc 
}

if [ $# -lt 1 ]; then
	echo "Usage: [server|client] [udp|tcp] <ipaddr> <pkg-size> <pkg-num>"
	exit
elif [ $# -eq 1 ]; then
	peer_type=$1
	protocol=$default_protocol
	serveraddr=$default_serveraddr
	pkg_size=$default_pkg_size
	pkg_num=$default_pkg_num
elif [ $# -eq 2 ]; then
	peer_type=$1
	protocol=$2
	serveraddr=$default_serveraddr
	pkg_size=$default_pkg_size
	pkg_num=$default_pkg_num
elif [ $# -eq 3 ]; then
	peer_type=$1
	protocol=$2
	serveraddr=$3
	pkg_size=$default_pkg_size
	pkg_num=$default_pkg_num
elif [ $# -eq 4 ]; then
	peer_type=$1
	protocol=$2
	serveraddr=$3
	pkg_size=$4
	pkg_num=$default_pkg_num
elif [ $# -eq 5 ]; then
	peer_type=$1
	protocol=$2
	serveraddr=$3
	pkg_size=$4
	pkg_num=$5
fi

if [ $peer_type = "server" ]; then
	server_dupkg $protocol
elif [ $peer_type = "client" ]; then
	client_dupkg $protocol $serveraddr $pkg_size $pkg_num
fi


