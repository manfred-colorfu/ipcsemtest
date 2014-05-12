/*
 * Copyright (C) 1999, 2014 by Manfred Spraul.
 * 
 * Redistribution of this file is permitted under the terms of the GNU 
 * General Public License (GPL)
 */

#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SEMS	5

int main(int argc,char** argv)
{
	int id, res, total;
	unsigned int i;

	printf("ncnt\n");
	printf("\n");
	printf(" This application tests how the OS implements GETNCNT for operation\n");
	printf(" that cannot proceed for multiple reasons: Is a task reported as sleeping\n");
	printf(" on all semaphores that to not match the required value (e.g. Linux) or\n");
	printf(" just on one semaphore (e.g. standard sysv code).\n");
	printf("\n");

	res = semget(IPC_PRIVATE, SEMS, 0700 | IPC_CREAT);
	printf(" got semaphore array %xh.\n",res);
	if(res == -1) {
		printf(" create failed.\n");
		return 1;
	}
	id = res;

	/* create sub-process */
	fflush(stdout);
	res = fork();
	if (res < 0) {
		printf("Fork failed (errno=%d). Aborting.\n", errno);
		res = semctl(id,1,IPC_RMID,NULL);
		exit(1);
	}
	if (!res) {
		/* child: */
		struct sembuf sop[SEMS];

		/* child: */
		for (i=0;i<sizeof(sop)/sizeof(sop[0]);i++) {
			sop[i].sem_num=i;
			sop[i].sem_op=-1;
			sop[i].sem_flg=0;
		}
		res = semop(id,sop,sizeof(sop)/sizeof(sop[0]));
		return 0;
	}
	sleep(1);

	total = 0;
	for (i=0;i<SEMS;i++) {
		res = semctl(id,i,GETNCNT,NULL);

		printf("semaphore %d: GETNCNT %d.\n", i, res);
		if (res >= 0)
			total += res;
	}
	printf("\n");
	printf(" Total: OS reports GETNCNT %d.\n", total);
	printf("\n");
	printf(" Expected: Linux: %d, other OS: 1.\n", SEMS);
	res = semctl(id,0,IPC_RMID,NULL);

	return 0;
}
