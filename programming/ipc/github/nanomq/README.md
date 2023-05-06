NanoMQ
======

NanoMQ is a ultra low latency messaging kernel. It enables messaging
between processes in much the same way as POSIX message queues but at
sub-microsecond latencies. NanoMQ uses efficient wait-free ring
buffers arranged in a complete graph. Each node can send messages to
any other node, receiving nodes needs to exclusively own a CPU core or
HyperThread. The ultra low latency can thus be achieved by avoiding
context switches.

Building
--------

Just run make. Requires recent GCC. Tests require Google Test.

Performance
-----------

On my Intel(R) Core(TM)2 Duo CPU T7500 @ 2.20GHz I get an average RTT
of 250 ns and a throughput of 13M msg/s for a two node setup with 100
byte messages. It will be interesting to see measurements on multi CPU
systems and the latency depending on which cache the cores share.

Use case
--------

In high frequency trading (HFT) systems you want to separate feed
handlers and order management systems (OMS) from strategy code in
order to increase fault tolerancy and support live deployment of bug
fixes or new strategies. NanoMQ allows you to separate these parts of
a trading system into separate processes while keeping communication
latencies to a fraction of a microsecond.

Resources
---------

Git repository: http://github.com/rigtorp/nanomq

Copying
-------

Free use of this software is granted under the terms of the GNU
General Public License (GPL). For details see the file `COPYING`
included with the NanoMQ distribution.
