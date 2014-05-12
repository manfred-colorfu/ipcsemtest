/*
 * Copyright (C) 1999 by Manfred Spraul.
 * 
 * Redistribution of this file is permitted under the terms of the GNU 
 * General Public License (GPL)
 * $Header: /home/manfred/cvs-tree/manfred/ipcsem/getall.c,v 1.2 2001/02/27 15:44:32 manfred Exp $
 */

#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define TRUE	1
#define FALSE	0

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo* __buf;
};


int main(int argc,char** argv)
{
	int id;
	int key;
	int res;
	int nsems;

	printf("getall (<id>|<-key>) [-v]\n");
	if(argc < 2 || argc > 3) {
		printf("Invalid parameters.\n");
		return 1;
	}
	if (argc == 3 && strcmp(argv[2], "-v")) {
		printf("Invalid parameters.\n");
		return 1;
	}
	key = atoi(argv[1]);
	if(key > 0) {
		id = semget(key,1,0);
		if(id == -1) {
			printf(" findkey() failed.\n");
			return 1;
		}
	} else {
		id = -key;
	}
	printf("Accessing semaphore array %d.\n", id);

	{
		union semun arg;
		struct semid_ds info;

		arg.buf = &info;
		res = semctl(id,0,2,arg);

		if(res==-1) {
			printf("semctl(2) failed (%d).\n",errno);
			return 1;
		}
		nsems = info.sem_nsems;
		printf("found %d semaphores.\n",nsems);
	}
	{
		union semun arg;
		unsigned short *pv;
		int i;

		pv = (unsigned short *)malloc(sizeof(unsigned short)*nsems);
		arg.array = pv;
		res = semctl(id,nsems, GETALL, arg);
		if(res==-1) {
			printf("GETALL failed.\n");
			return 1;
		}
		for(i=0;i<nsems;i++) {
			if (argc == 3) {
				printf(" %3d: %3d (cnt %3d zcnt %3d pid %3d)\n",
					i, pv[i], semctl(id, i, GETNCNT),
					semctl(id, i, GETZCNT),
					semctl(id, i, GETPID));
			} else {
				if(i%16 == 0)
					printf("\n 0x%4.4x:",i);
				printf(" %3d",pv[i]);
			}
		}
		printf("\n");
	}
	return 0;
}
