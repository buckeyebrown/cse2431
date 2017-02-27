#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full, empty;

int insert_item(buffer_item item);
int remove_item(buffer_item *item);

int main(int argc, char* argv[]){
	//get command line arguments argv[1], argv[2], argv[3]
	//How long to sleep before terminating
	const int timeSleep = atoi(argv[1]);
	//The number of producer threads
	const int numOfProducers = atoi(argv[2]);
	//The number of consumer threads
	const int numOfConsumers = atoi(argv[3]);

	//initialize buffer, mutex, semaphores, and global variables
	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, BUFFER_SIZE);
	sem_init(&empty, 1, BUFFER_SIZE);
	//create producer threads
	pthread_t producerThreads[numOfProducers];
	//create consumer threads
	pthread_t consumerThreads[numOfConsumers];
	//sleep
	//release resources, aka destroy mutex and semaphores

	return 0;
}

int insert_item(buffer_item item){
	int error_flag;
	//wait empty
	sem_wait(&empty);
	//wait mutex
	pthread_mutex_lock(&mutex);



	// insert item

	prinf("Producer produced%d\n", item);

	//ret 0 if successful
	//ret -1 if not
}

int remove_item(buffer_item *item){
	// remove object from buffer and place it in item

	printf("Consumer consumed%d\n", rand);

	//ret 0 if successful
	//ret -1 if not
}

void *producer(void *param){
	buffer_item rand;

	while(1){
		// sleep for a random period of time between 1 and 10 seconds
		sleep(rand() % 10 + 1);

		//generate a random number
		//rand = rand_r(..);

		if(insert_item(rand) < 0){
			//report error with printf
			printf("There was an error with the producer function");
		}
	}
}

void *consumer(void *param){
	buffer_item rand;

	while(1){
		//sleep for a random period of time
		// sleep(..)

		if(remove_item(&rand) < 0){
			//report error with printf
			printf("There was an error with the consumer function");
		}

	}
}