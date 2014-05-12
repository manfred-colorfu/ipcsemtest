/*
 * Copyright (C) 1999 by Manfred Spraul.
 * 
 * Redistribution of this file is permitted under the terms of the GNU 
 * General Public License (GPL)
 * $Header: /home/manfred/cvs-tree/manfred/ipcsem/createary.c,v 1.3 2003/06/17 16:16:55 manfred Exp $
 */

#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc,char** argv)
{
	int id, nsems, res, perms;

	printf("createary <id> <nsems> [<perms>]\n");
	if(argc < 3 || argc > 4) {
		printf("Invalid parameters.\n");
		return 1;
	}
	id = atoi(argv[1]);
	if(id <= 0) {
		printf("Invalid parameters.\n");
		return 1;
	}
	nsems = atoi(argv[2]);
	if(nsems <= 0) {
		printf("Invalid parameters.\n");
		return 1;
	}	
	if (argc > 3) {
		perms = atoi(argv[3]);
	} else {
		perms = 0777;
	}
	res = semget(id, nsems, perms | IPC_CREAT);
	if(res == -1) {
		printf(" create failed.\n");
		return 1;
	}
	return 0;
}
