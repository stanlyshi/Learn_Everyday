# Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:29 CST. 
 Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:02 CST. 
 Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:28:19 CST. 


# tab -> 4 space


   +----------+      autosca    +---------------+
   |sourceCode|---------------->| configure.scan|
   +----------+                 +---------------+
                                        | edit
                                       \|/
 +-------------+                +---------------+   aclocal    +-------------+
 | Makefile.am |                | configure.ac  |------------->| aclocal.m4  |
 +-------------+                +---------------+              +-------------+
        |                              |         \                   |
automake --add-missing             autoheader     \----------------->|
        |                              |                             | autoconf
       \|/                            \|/                           \|/
 +-------------+               +---------------+               +------------+
 | Makefile.in |               | config.h.in   |               | configure  |
 +-------------+               +---------------+               +------------+
        \                              |                             /
         \                             |                            /
          ---------------------------->|<---------------------------
                                       |
                                       | configure
                                      \|/
                                  +----------+
                                  | Makefile |
                                  +----------+


1.autoscan
    configure.scan

2.edit
    configure.scan -> configure.ac

3.aclocal
    aclocal.m4

4.autoconf
    configure

5.autoheader
    config.h.in

6.Makefile.am
    edit a new Makefile.am

7.automake 
    automake --add-missing

8. ...

9.package
    make dist && make distclean


