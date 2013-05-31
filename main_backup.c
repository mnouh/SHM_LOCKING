#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_SIZE 1024

typedef struct message_struct{
	int flag;
	int integer1;
	int integer2;
	sem_t s;
	sem_t t;
	sem_t x;
} msg;

int main(int argc, char ** argv)
{
int pid;
int pid2;
key_t key = 123456789;
int shmid;
char* data;
int integer1;
int integer2;
msg* message;
	if(argc != 3) 
	{
		printf("Incorrect Number of Arguments");
	exit(1);
	}

		//Create segment
		if ((shmid = shmget(key, sizeof(*message), 0777 | IPC_CREAT| IPC_EXCL )) < 0) {
			perror("Unable to create segment");
			exit(1);
		}
		//Attach to segment
		message = shmat(shmid, (void *)0, 0);
		if (message == (void *)(-1)) {
			perror("Unable to attach to segment");
			exit(1);
		}

	if(sem_init(&message->s, 1, 0) < 0) 
	{
	perror("failed to initialize semaphore");
	exit(1);			
	}

	if(sem_init(&message->t, 1, 0) < 0) 
	{
	perror("failed to initialize semaphore");
	exit(1);			
	}
	
	integer1 = atoi(argv[1]);
	integer2 = atoi(argv[2]);

	
	if(sem_init(&message->s, 1, 1) < 0) 
	{
	perror("failed to initialize semaphore");
	exit(1);			
	}
	

	//printf("%d\n %d\n", integer1, integer2);
	pid = fork();
	pid2 = fork();
	if(pid > 0 && pid2 > 0)
	{



			message->integer1 = integer1;
			message->integer2 = integer2;
//s is consumer
//t is producer
			
			sem_post(&message->s);
			sem_post(&message->x);
			sem_wait(&message->t);

			printf("Parent says Product: %d\n", message->integer1);
			printf("Parent says Sum: %d\n", message->integer2);

	}
	else if(pid == 0) {

	//Connect to segment

		sem_wait(&message->s);
		sem_wait(&message->x);
		//Write message to segment
		//printf("Child Reading Values: ");
		//printf("Integer1: %d\n", message->integer1);
		//printf("Integer2: %d\n", message->integer2);
		int tmp = integer1 * integer2;
		message->integer1 = tmp;
		//printf("Child says Product: %d\n", message->integer1);
		
		//sem_post(&message->x);
		sem_post(&message->x);		
		sem_post(&message->t);
		

		

	}
	if(pid2 == 0) {

	sem_wait(&message->s);
	sem_wait(&message->x);
	
	int temp2 = integer1 + integer2;
	message->integer2 = temp2;
	
	sem_post(&message->s);
	sem_post(&message->t);
	
}

	sem_destroy(&message->s);
	sem_destroy(&message->x);
	sem_destroy(&message->t);
return 0;

}
