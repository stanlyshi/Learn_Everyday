


*** Sniffing Unix Domain Sockets: 嗅探Unix域套接字 ***

	----------------------------------------------------------------------------------------------------------------------
	https://mivehind.net/2018/04/20/sniffing-unix-domain-sockets/
	Apr 20, 2018 [ tech ]
	A quick post because I can't resist the "sniffing" joke.
	If you do a lot of network traffic analysis, you've probably used wireshark, tcpdump, and other tools of that nature. 
	However, they only work well for network traffic - try to use Unix domain sockets and you're out of luck - they don't 
	understand them.
	Unless you proxy the traffic. Like so:
	First, let the process open whatever socket you care about.
	Then, move it out of the way and have socat listen in its stead:

	>> mv DEFAULT.socket hidden.socket
	>> socat UDP-LISTEN:6000,reuseaddr,fork UNIX-CONNECT:hidden.socket

	This sets up socat to take anything from UDP port 6000 and apply it to the (now hidden) socket, with it none the wiser.
	Then we plug tcpdump in to listen on this port:

	>> tcpdump -ni lo -s0 -f 'udp port 6000' -w /tmp/out.pcap

	And set up the proxy entry where the socket was expecting to be:

	>> socat UNIX-LISTEN:DEFAULT.socket,fork UDP-CONNECT:127.0.0.1:6000

	-------------------------------------------------------------------------------------------------------
	// backup the socket
	sudo mv /var/run/docker.sock /var/run/docker.sock.original

	// use tcp port 8089 proxy the original socket
	sudo socat TCP-LISTEN:8089,reuseaddr,fork UNIX-CONNECT:/var/run/docker.sock.original

	// use the new socket to proxy the 8089 port
	sudo socat UNIX-LISTEN:/var/run/docker.sock,fork TCP-CONNECT:127.0.0.1:8089


	------------------------------------------------------------------------------------------------------------
	#!/bin/bash

	mv rtoax_unsocket_pcap rtoax_unsocket_pcap.original
	socat TCP-LISTEN:8089,reuseaddr,fork UNIX-CONNECT:./rtoax_unsocket_pcap.original
	socat UNIX-LISTEN:./rtoax_unsocket_pcap,fork TCP-CONNECT:127.0.0.1:8089
	tshark -i lo -w cap.pcap -F pcapng "dst port 8089 or src port 8089"



	------------------------------------------------------------------------------------------------------------

	#!/bin/bash
	# 荣涛 2020年11月9日
	# UMIX socket抓包工具

	exe_file_name=$0

	pcapfile=cap.pcap
	pcapfile_flag=0

	tcpport=8087

	function usage() {
		echo "$exe_file_name [unix path to pcap] [capture.pcap] [tcp-swap-port]"
	}

	# 参数
	if [ $# -eq 1 ]; then
	    unix_path_to_dump=$1
	elif [ $# -eq 2 ]; then
	    unix_path_to_dump=$1
	    pcapfile=$2
	elif [ $# -eq 3 ]; then
	    unix_path_to_dump=$1
	    pcapfile=$2
	    tcpport=$3
	else
	    usage
	    exit 1
	fi

	rm -f $pcapfile

	source_unix_path_to_dump="$unix_path_to_dump.original"

	echo unix_path_to_dump=$unix_path_to_dump
	echo source_unix_path_to_dump=$source_unix_path_to_dump
	echo tcpport=$tcpport
	echo pcapfile=$pcapfile

	# Move socket files
	mv "${unix_path_to_dump}" "${source_unix_path_to_dump}"
	trap "{ rm '${unix_path_to_dump}'; mv '${source_unix_path_to_dump}' '${unix_path_to_dump}'; }" EXIT

	#创建一个TCP监听，一个UNIXSocket监听
	socat -t100 "TCP-LISTEN:${tcpport},reuseaddr,fork" "UNIX-CONNECT:${source_unix_path_to_dump}" &

	#创建一个UNIX监听和一个TCP监听
	socat -t100 "UNIX-LISTEN:${unix_path_to_dump},mode=777,reuseaddr,fork" "TCP:localhost:${tcpport}" &


	#保存抓包数据
	tcpdump -i lo -netvvv port $tcpport -w "${pcapfile}"

	#显示包数据
	#tcpdump -i lo -netvvv -x port $tcpport 