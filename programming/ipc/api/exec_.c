/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:04 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:58 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:39 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:11 CST. */
/**
 *	存放在硬盘中的可执行文件要想运行就必须调用exec函数中的某一个
 *	统称为exec函数，称调用exec函数的进程为调用进程，
 *				称新执行的程序为新程序
 *	这6个exec函数之间的区别在于：
 *		1：待执行的程序文件值文件名还是路径名
 *		2：新程序的参数是一一列出还是由一个指针数组来引用
 *		3：把用进程的环境传递给新程序还是给新程序指定新的环境
 *
 *	#include <unistd.h>
 *
 *	int execl(const char *pathname,
 *				const char *arg0,...);
 *	int execv(const char *pathname,
 *				char *const *argv[]);
 *	int execle(const char *pathname,
 *				const char argv0,...);
 *	int execve(const char *pathname,
 *				char *const argv[],
 *				char *const envp[]);
 *	int execlp(const char *filename,
 *				char *char *arg0, ...);
 *	int execvp(const char *filename,
 *				char *const argv[]);
 */
