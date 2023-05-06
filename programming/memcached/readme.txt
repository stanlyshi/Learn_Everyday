memory cached


安装位置
/usr/bin/mkdir -p '/usr/local/share/man/man1'
/usr/bin/install -c -m 644 memcached.1 '/usr/local/share/man/man1'
/usr/bin/mkdir -p '/usr/local/bin'
/usr/bin/install -c memcached '/usr/local/bin'
/usr/bin/mkdir -p '/usr/local/include/memcached'
/usr/bin/install -c -m 644 protocol_binary.h '/usr/local/include/memcached'