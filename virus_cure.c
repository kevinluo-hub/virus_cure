#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// semaphore that allows only up to 8 calls to run at a time
static sem_t connected_lock;
// semaphore that allows only up to 5 operation to run at a time
static sem_t operators;
// binary semaphore, used for integer increment
static sem_t binary;
// static int stating there's 5 operators
static int NUM_OPERATORS = 5;
// static int stating there's 8 call lines
static int NUM_LINES = 8;
// static int that represents the number of connected calls at the moment
static int connected = 0;
// int that represents the current call IDs
int next_id = 0;

//thread function that simulates the whole call process
void* phonecall(void* vargp);

//starts at main inside for loops 
int main(int argc, char* argv[]) {
	//semaphore initializations
	sem_init(&connected_lock, 0, NUM_LINES);
	sem_init(&operators, 0, NUM_OPERATORS);
	sem_init(&binary, 0, 1);

	//getting amount of calls from argument
	int times = atoi(argv[1]);

	//initializing array that stores the thread IDs that are currently on call
	pthread_t call_list[NUM_LINES];
	for (int i = 0; i < NUM_LINES; i++)
		call_list[i] = (pthread_t)NULL;

	//temp variable to catch thread ID
	pthread_t tid;

	//making calls by creating threads and storing them into the array
	for (int i = 0; i < times; i++) {
		pthread_create(&tid, NULL, phonecall, NULL);
		if (call_list[i%NUM_LINES])
			pthread_join(call_list[i%NUM_LINES], NULL);
		call_list[i%NUM_LINES] = tid;
	}

	//block main thread until other call threads are completed
	for (int i = 0; i < NUM_LINES; i++) {
		if (call_list[i%NUM_LINES]) {
			pthread_join(call_list[i%NUM_LINES], NULL);
		}
	} 

	//destroy semaphores that are created after calls are done
	sem_destroy(&connected_lock);
	sem_destroy(&operators);
	sem_destroy(&binary);

	//print message to show that all calls are completed and all threads are properly terminated
	printf("\nALL CALLS ARE COMPLETED.\n");
	return 0;
}

void* phonecall(void* vargp) {
	//update next_id to give correct call IDs
	sem_wait(&binary);
	int n = ++next_id;
	sem_post(&binary);
	
	printf("Thread [%d] is attempting to connect ...\n", n);

	//test connected
	if (connected >= NUM_LINES)
		printf("Line is busy!\n");

	//calling process
	sem_wait(&connected_lock);
	printf("Thread [%d] connects to an available line, call ringing ...\n", n);
	sem_wait(&binary);
	connected++;
	sem_post(&binary);
	sem_wait(&operators);
	printf("Thread [%d] is speaking to an operator.\n", n);
	usleep(1000000);
	printf("Thread [%d] has ordered a medicine! The operator has left ...\n", n);
	sem_post(&operators);
	sem_wait(&binary);
	connected--;
	sem_post(&binary);
	printf("Thread [%d] has hung up!\n", n);
	sem_post(&connected_lock);
}
