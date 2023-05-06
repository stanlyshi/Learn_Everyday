https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux_for_real_time/7/html/tuning_guide/rocee_and_high_performance_networking

RoCEE（基于聚合增强型以太网的RDMA）是一种协议，可在10千兆位以太网上实现远程直接内存访问（RDMA）。它使您可以在数据中心中维护一致的高速环境，同时为关键事务提供确定的，低延迟的数据传输。
高性能网络（HPN）是一组共享库，它们向内核提供RoCEE接口。HPN无需通过独立的网络基础架构，而是使用标准的10 Gb以太网基础架构将数据直接放置到远程系统内存中，从而减少了CPU开销并降低了基础架构成本。
在Red Hat Enterprise Linux for Real Time下对RoCEE和HPN的支持与在Red Hat Enterprise Linux 7下提供的支持没有区别。
