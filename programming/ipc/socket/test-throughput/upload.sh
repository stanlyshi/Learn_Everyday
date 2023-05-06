#!/bin/bash
# scp拷贝文件自动密码
# rongtao@sylincom.com

function upload() {
set password 123456

spawn scp -r ../test-throughput/ 10.170.7.166:/home
set timeout 300
expect "root@10.170.7.166's password:"
set timeout 300
send "$password\r"
set timeout 300
send "exit\r"
expect eof
}



scp -r ../test-throughput/ $1:/home




