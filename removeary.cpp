/*
 * Copyright (C) 1999 by Manfred Spraul.
 * 
 * Redistribution of this file is permitted under the terms of the GNU 
 * General Public License (GPL)
 * $Header: /home/manfred/cvs-tree/manfred/ipcsem/removeary.c,v 1.3 2003/06/17 16:16:55 manfred Exp $
 */

#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo* __buf;
};

int main(int argc,char** argv)
{
	int id;
	int res;
	union semun arg;

	printf("removeary <id>\n");
	if(argc != 2) {
		printf("Invalid parameters.\n");
		return 1;
	}
	id = atoi(argv[1]);
	if(id <= 0) {
		printf("Invalid parameters.\n");
		return 1;
	}
	res = semget(id,1,0);
	if(res == -1) {
		printf(" open failed.\n");
		return 1;
	}
	id = res;
	res = semctl(id,1,IPC_RMID,arg);
	if(res == -1) {
		printf(" semctl failed.\n");
		return 1;
	}
return 0;
}
