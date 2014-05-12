/*
 * Copyright (C) 1999 by Manfred Spraul.
 * 
 * Redistribution of this file is permitted under the terms of the GNU 
 * General Public License (GPL)
 * $Header: /home/manfred/cvs-tree/manfred/ipcsem/dec.c,v 1.5 2003/06/17 16:16:55 manfred Exp $
 */

#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
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
	int *nr;
	int *val;
	int i;
	int sems;

	printf("change <id> <semnum> <change> [...]\n");
	if(argc < 4 || ((argc % 2) == 1)) {
		printf("Invalid parameters.\n");
		return 1;
	}
	
	key = atoi(argv[1]);
	if(key == 0) {
		printf("Invalid parameters: Key invalid.\n");
		return 1;
	}
	if (key > 0) {
		id = semget(key,1,0);
		if(id == -1) {
			printf(" findkey() failed.\n");
			return 1;
		}
	} else {
		id = -key;
		printf(" findkey() bypassed, using id %d.\n", id);
	}
	sems = (argc-2)/2;
	nr=(int*)malloc(sizeof(int)*sems);
	val=(int*)malloc(sizeof(int)*sems);
	if (!nr || !val) {
		printf("Malloc failed.\n");
		return 1;
	}
	printf("pid %d: changing %d semaphores:\n",getpid(), sems);
	for (i=0;i<sems;i++) {
		nr[i] = atoi(argv[2+2*i]);
		val[i] = atoi(argv[3+2*i]);
		printf("       sem %3d by %2d\n",nr[i],val[i]);
	}


	{
		struct sembuf *sop;

		sop = (struct sembuf *)malloc(sizeof(*sop)*sems);
		if (!sop) {
			printf("malloc() failed.\n");
			return 1;
		}
		for (i=0;i<sems;i++) {
			sop[i].sem_num=nr[i];
			sop[i].sem_op=val[i];
			sop[i].sem_flg=0;
		}

		res = semop(id,sop,sems);
		if(res==-1) {
			printf("pid %d: semop() failed, errno %d.\n", getpid(), errno);
			return 1;
		}
	}
	printf("pid %d: semop() successful.\n", getpid());
	return 0;
}
