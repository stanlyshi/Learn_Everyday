#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdio.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main()
{
	char *filename = "msgq1";

	int flags = O_RDWR | O_CREAT | O_EXCL;
	struct mq_attr	attr;
	attr.mq_maxmsg = 100;
	attr.mq_msgsize = 1024;

	mqd_t mqd = mq_open(filename, flags, FILE_MODE, &attr);


	mq_close(mqd);
	return 0;
}

