#include <unistd.h>

#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include <sys/shm.h>


#include "CSORT.h"

#include <sys/sem.h>


static int set_semvalue(int index);
static void del_semvalue(int index);
static int semaphore_p(int index);
static int semaphore_v(int index);

static int sem_id1;
static int sem_id2;
static int sem_id3;
static char temp;



int main(){

	
	char g='g';// green color
	char r='r';//red color
	char c;
	pid_t pid;
	struct shared_use_st *arrays;
	int shmid;
	int status;
	void *shared_memory = (void *)0;
	int debug=0;
	sem_id1 = semget((key_t)1234, 1, 0666 | IPC_CREAT); //the 1st semaphore is created with a call to semget, and stores the id  . 				
	sem_id2 = semget((key_t)1234, 1, 0666 | IPC_CREAT);//the 2nd semaphore is created with a call to semget , and stores the id . 
	sem_id3 = semget((key_t)1234, 1, 0666 | IPC_CREAT);//the  3rd semaphore is created with a call to semget , and stores the id .
	set_semvalue(sem_id1);
	set_semvalue(sem_id2);
	set_semvalue(sem_id3);
	
	
	
	
	
	
	
	
	
	// cresting shared memory	shmget creates a shared memory
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if (shmid == -1) { // error handling 

		fprintf(stderr, "shmget failed\n");

		exit(EXIT_FAILURE);

	}
	
	// making shared memory accessible to the program shmat() returns a pointer to void
	shared_memory = shmat(shmid, (void *)0, 0);

	if (shared_memory == (void *)-1) {

		fprintf(stderr, "shmat failed\n");

		exit(EXIT_FAILURE);

	}
	//casting of type struct
	arrays = (struct shared_use_st *)shared_memory;
	printf("Do you want debug mode enter 1 for yes or 0 for no\n");// ask to debug
	scanf("%d",&debug);
	int k;
	for(k=0;k<10;k++){// collects user input
		printf("Enter a red or green object were r represents [red] and g represents [green]\n");
		if(k<5){
			scanf(" %c",&arrays->R[k]);
			
		}
		
		else{
			static int n=0;
			
			scanf(" %c",&arrays->G[n]);
			n++;
			
			
		}
		
	}
	for(int l=0;l<10;l++){// shows  user iput

		if(l<5){
			printf(" %c\n",arrays->R[l]);
			
		}
		
		else{
			static int z=0;
			printf(" %c\n",arrays->G[z]);
			z++;
			
			
		}
		
	}
	
	




	arrays->flag=1;//flag to check if all the processes are done  sorting or not
	
	int i=0;

	for(;i<4;i++){ //creates 4 processess 
		pid=fork();
		if(pid==0){
			break;
		}
	}

	switch(pid){
		case -1:
			printf("fork failed");
			exit(0);
		case 0:
			if(i==0){
				
				while(arrays->flag){
					
					if ((arrays->R[0]==g && arrays->R[1]==r )){
						temp=arrays->R[1];
						arrays->R[1]=arrays->R[0];
						arrays->R[0]=temp;
						
						
					}
					semaphore_p(sem_id1);
					 if(arrays->R[1]==g && arrays->R[2]==r ){
						temp=arrays->R[1];
						arrays->R[1]=arrays->R[2];
						arrays->R[2]=temp;
							
							
							
							
						
					}
					semaphore_v(sem_id1);
					
					if(debug==1){
						printf("%c , %c, %c,%c , %c",arrays->R[0],arrays->R[1],arrays->R[2],arrays->R[3],arrays->R[4] );
						printf(" 0 child working\n ");
					}
					
					
					
					sleep(1);
				
				}
			

			}
			else if(i==1){
				
				while(arrays->flag){
					
					semaphore_p(sem_id1);
					if ((arrays->R[2]==g && arrays->R[3]==r )){
						
						temp=arrays->R[3];
						arrays->R[3]=arrays->R[2];
						arrays->R[2]=temp;
						
						
					}
					semaphore_v(sem_id1);
					
					
					semaphore_p(sem_id2);
					 if(arrays->R[4]==r && arrays->R[3]==g ){
						temp=arrays->R[4];
						arrays->R[4]=arrays->R[3];
						arrays->R[3]=temp;
							
							
							
						}
					
					
					semaphore_v(sem_id2);
				if(debug==1){
					printf("%c , %c, %c,%c , %c",arrays->R[0],arrays->R[1],arrays->R[2],arrays->R[3],arrays->R[4] );
					printf(" 1 child working\n ");
					}
				
					sleep(1);
				
				}
				
				
			}
			else if(i==2){
				
				while(arrays->flag){
					
					semaphore_p(sem_id2);
					if ((arrays->R[4]==g && arrays->G[0]==r  )){
						
						temp=arrays->R[4];
						arrays->R[4]=arrays->G[0];
						arrays->G[0]=temp;
						
						
					}
					semaphore_v(sem_id2);
					 if(arrays->G[0]==g && arrays->G[1]==r ){
						
						arrays->G[0]=temp;
						arrays->G[0]=arrays->G[1];
						arrays->G[1]=temp;
						
					
							
							
						}
					
					semaphore_p(sem_id3);
					 if(arrays->G[1]==g && arrays->G[2]==r){
						temp=arrays->G[1];
						arrays->G[1]=arrays->G[2];
						arrays->G[2]=temp;
					
					
					
					
				
					}
					semaphore_v(sem_id3);
					if(debug==1){
						printf("%c , %c, %c,%c , %c",arrays->G[0],arrays->G[1],arrays->G[2],arrays->G[3],arrays->G[4] );
						printf(" 2 child working\n ");
					}
					
					sleep(1);
				}
				
				
				
			}
			else if(i==3){
				
				while(arrays->flag){
					semaphore_p(sem_id3);
					
					if ((arrays->G[2]==g && arrays->G[3]==r )){
						
						temp=arrays->G[2];
						arrays->G[2]=arrays->G[3];
						arrays->G[3]=temp;
						
						
					}
					semaphore_v(sem_id3);
					
					 if(arrays->G[3]==g && arrays->G[4]==r){
						
							
							temp=arrays->G[3];
							arrays->G[3]=arrays->G[4];
							arrays->G[4]=temp;
							
							
							
						
					}
					
					if(debug==1){
						printf("%c , %c, %c,%c , %c",arrays->G[0],arrays->G[1],arrays->G[2],arrays->G[3],arrays->G[4] );
						printf(" 3 child working\n ");
					}
					
					
					
					sleep(1);
				
				
				}
				
				
			}
			
			else{
				printf("error wrong"); 
			}
			//child detaches from  the shared memory
			if (shmdt(shared_memory) == -1) {

				fprintf(stderr, "shmdt failed\n");

				exit(EXIT_FAILURE);

			}
			exit(0);
			
		default:
			
			
			while(1){
				if(arrays->R[0]==r&&arrays->R[1]==r &&arrays->R[2]==r &&arrays->R[3]==r &&arrays->R[4]==r &&arrays->G[0]==g &&arrays->G[1]==g&&arrays->G[2]==g&&arrays->G[3]==g&&arrays->G[4]==g ){
					arrays->flag=0;
					for(int p=0;p<10;p++){

						if(p<5){
							printf(" %c\n",arrays->R[p]);
							
						}
						
						else{
							static int m=0;
							printf(" %c\n",arrays->G[m]);
							m++;
							
							
						}
		
	}
					
					
					
					
					break;
				}
				else{
					
					sleep(5);
					printf("still sorting\n");
					
					
				}
				
			}
			
			
			
			break;
			
			
			
			
			
	//parent detaches from  the shared memory and deletes it
			if (shmdt(shared_memory) == -1) {

				fprintf(stderr, "shmdt failed\n");

				exit(EXIT_FAILURE);

			}
			if (shmctl(shmid, IPC_RMID, 0) == -1) {

				fprintf(stderr, "shmctl(IPC_RMID) failed\n");

				exit(EXIT_FAILURE);

			}
			




	}
}

/*
initializes the semaphore using the SETVAL command, need to do this before you can use the semaphore
*/
static int set_semvalue(int index)
{
	union semun sem_union;
	sem_union.val = 1;
	if (semctl(index, 0, SETVAL, sem_union) == -1) return(0);
	return(1);
}

/*
uses the command IPC_RMID to remove the semaphore’s ID
*/
static void del_semvalue(int index)
{
	union semun sem_union;
	if (semctl(index, 0, IPC_RMID, sem_union) == -1)
	fprintf(stderr, "Failed to delete semaphore\n");
}

/*
semaphore_p changes the semaphore by –1. This is the “wait” operation:
if semaphore is 0, it will wait
*/
static int semaphore_p(int index)
{
	struct sembuf sem_b;
	sem_b.sem_num = index;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(index, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_p failed\n");
		return(0);
	}
	return(1);
}
/*
semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1. This is
the “release” operation, so that the semaphore becomes available
*/
static int semaphore_v(int index)
{
	struct sembuf sem_b;
	sem_b.sem_num = index;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(index, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_v failed\n");
		return(0);
	}
	return(1);
}


	
	
		
	
		
		
	
	
	
