#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <time.h>
#include "semun.h"

int
main(int argc, char *argv[]){
	
	pid_t cid, pid;
	int semid;
	union semun arg;
	struct sembuf sop;
	struct timespec timespc;

	semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
	if(semid == -1)
		return -1;
	arg.val = 0;
	if (semctl(semid, 0, SETVAL, arg) == -1)
		return -1;
	printf("Semaphore ID = %d\n", semid);

	sop.sem_num = 0;
	sop.sem_flg = 0;
	timespc.tv_sec = 1;

	if((cid = fork()) == 0){
		//child
		pid = getpid();
		for(;;){	
			sop.sem_op = -1;
			if (semop(semid, &sop, 1) == -1)
				return -1;	
			printf("%d tock\n", pid);
			nanosleep(&timespc, NULL);
			sop.sem_op = 1;
			if (semop(semid, &sop, 1) == -1)
				return -1;	
			printf("%d tick\n", pid);
		}
		
	}
	else{
		pid = getpid();
		for(;;){	
			nanosleep(&timespc, NULL);
			sop.sem_op =  1;
			if (semop(semid, &sop, 1) == -1)
				return -1;	
			printf("%d tick\n", pid);
			nanosleep(&timespc, NULL);
			sop.sem_op = -1;
			if (semop(semid, &sop, 1) == -1)
				return -1;	
			printf("%d tock\n", pid);
		}
	}
	return 0;
}
