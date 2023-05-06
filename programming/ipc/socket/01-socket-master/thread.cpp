/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:11 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:02 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:43 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:14 CST. */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

class Parent {
public:
	Parent() : p_status(&status), thread_enable(0) {}
	void start_server() {
		int ret = pthread_create(&tid, NULL, thread_func, NULL);
		if (ret == 0)
			printf("Start a thread\n");
		else
			printf("Can't start a thread: %s\n", strerror(ret));
	}
	static void *thread_func(void *data) {
		while (state == 1) {
			printf("In thread function, %d\n", count++);
			sleep(1);
		}
		return NULL;
	}
	void stop() {
		state = 0;
		if (pthread_join(tid, (void**)p_status) == 0)
			printf("End a thread\n");
		else
			printf("Can't end a thread: %d\n", status);
	}
private:
	static int state, count;
	pthread_t tid;
	int status, *p_status;
	int thread_enable;
};

int Parent::state = 1;
int Parent::count = 0;

class A : public Parent {
public:
	void start() {
		Parent::start_server();
	}
	void stop() {
		Parent::stop();
	}
};

int main()
{
	A a;
	a.start();
	sleep(3);
	a.stop();
	return 0;
}

