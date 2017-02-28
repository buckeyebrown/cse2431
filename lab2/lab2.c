/*
* Dylan Brown
* CSE 2431
* Lab 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "buffer.h"

//global vars
buffer_item buffer[BUFFER_SIZE];
//mutex lock
pthread_mutex_t mutex;
//semaphores
sem_t full, empty;
int counter, in, out;

//function prototypes
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *param);
void *consumer(void *param);

//main function
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
	//Set the full semaphore with a value of 0
	sem_init(&full, 0, 0);
	//Set the empty buffer with a value of BUFFER_SIZE
	sem_init(&empty, 0, BUFFER_SIZE);
	//Initialize counter, in, and out to 0
	counter = 0;
	in = 0;
	out = 0;

	//create producer threads
	pthread_t producerThreads[numOfProducers];
	pthread_attr_t attr;

	//Get default attribute
	pthread_attr_init(&attr);
	int a = 0;
	while (a < numOfProducers){
		pthread_create(&producerThreads[a], &attr, producer, NULL);
		a++;
	}

	//create consumer threads
	pthread_t consumerThreads[numOfConsumers];
	a = 0;
	while (a < numOfConsumers){
		pthread_create(&consumerThreads[a], &attr, consumer, NULL);
		a++;
	}

	//sleep
	sleep(timeSleep);

	//release resources, aka destroy mutex and semaphores
	sem_destroy(&full);
	sem_destroy(&empty);
	pthread_mutex_destroy(&mutex);

	//exit
	return 0;
}

//producer process
int insert_item(buffer_item item){
	int error_flag;
	//wait empty
	sem_wait(&empty);
	//wait mutex
	pthread_mutex_lock(&mutex);

	// insert item / Add next produced to buffer
		while(counter == BUFFER_SIZE){

		}
		if(counter != BUFFER_SIZE){
			buffer[in] = item;
			in = (in + 1) % BUFFER_SIZE;
			counter++;
			error_flag = 0;
		}
		else{
			error_flag = -1;
		}
	
	//signal mutex and full
	pthread_mutex_unlock(&mutex);
	sem_post(&full);

	//ret 0 if successful
	//ret -1 if not

	return error_flag;
}

//consumer process
int remove_item(buffer_item *item){
	int error_flag;
	// remove object from buffer and place it in item
	//wait full
	sem_wait(&full);
	//wait mutex
	pthread_mutex_lock(&mutex);
		while(counter == 0){
		}
		// remove item / consume item from buffer
		if (counter != 0){
			*item = buffer[out];
			out = (out + 1) % BUFFER_SIZE;
			counter--;
			error_flag = 0;
		}
		else{
			error_flag = -1;
		}
	//signal mutex and empty
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	//ret 0 if successful
	//ret -1 if not
	return error_flag;
}

//Entry point of producer thread
void *producer(void *param){
	buffer_item randVal;
	unsigned int seed = (time(NULL) % 20) + 1;
	
	while(1){
		// sleep for a random period of time between 1 and 3 seconds
		sleep((rand() % 3) + 1);

		//generate a random number from 1 to 5000
		randVal = rand_r(&seed) % 5000;
		if(insert_item(randVal) < 0){
			//report error with printf
			printf("There was an error with the producer function");
		}
		else{
			printf("Producer produced: %d\n", randVal);
		}
	}
}

//Entry point of consumer thread
void *consumer(void *param){
	buffer_item randVal;

	while(1){
		//sleep for a random period of time
		// sleep for a random period of time between 1 and 10 seconds
		sleep((rand() % 3) + 1);

		if(remove_item(&randVal) < 0){
			//report error with printf
			printf("There was an error with the consumer function");
		}
		else{
			printf("Consumer consumed: %d\n", randVal);
		}
	}
}