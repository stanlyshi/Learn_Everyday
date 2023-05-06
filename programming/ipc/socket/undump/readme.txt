# UNIX域套接字抓包工具
# 荣涛
# 2020年11月9日

---------------------------------------------------------------

1.安装socat（已安装的，请忽略此步骤）

2.编译测试demo

	cd demo
	make

3.启动服务端（5G基站对应OM程序）

	./server

4.启动脚本（undump.sh -h查看帮助信息）

	./undump.sh -u demo/rtoax_unsocket_pcap -x -p 9872

	参数说明（更多参数请使用 ./undump.sh -h 查询）
	-u UNIX域套接字路径，demo/rtoax_unsocket_pcap 在代码中写死
		若为5G基站程序，该值对应下列值（顾名思义，不做解释）
		/etc/jt_product/jt_sran/un_sock/CUCP_UNSOCKET_PATH
		/etc/jt_product/jt_sran/un_sock/CUUP_UNSOCKET_PATH
		/etc/jt_product/jt_sran/un_sock/DUL2_UNSOCKET_PATH
		/etc/jt_product/jt_sran/un_sock/DUPHY_UNSOCKET_PATH
		/etc/jt_product/jt_sran/un_sock/SIMU_CUCP_UNSOCKET_PATH
		/etc/jt_product/jt_sran/un_sock/SIMU_CUUP_UNSOCKET_PATH
		/etc/jt_product/jt_sran/un_sock/SIMU_DUL2_UNSOCKET_PATH

	-x 当命令参数指定时，抓包内容会打印出16进制报数据
	-p 有时会出现 Address already in use，此时可执行该值（未被占用的port）

5.启动客户端（5G基站对应协议软件）

	./client

6.观察抓包数据即可。
