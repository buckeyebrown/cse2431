#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];

int insert_item(buffer_item item);
int remove_item(buffer_item *item);

int main(int argc, char* argv[]){
	//get command line arguments argv[1], argv[2], argv[3]
	//initialize buffer, mutex, semaphores, and global variables
	//create producer threads
	//create consumer threads
	//sleep
	//release resources, aka destroy mutex and semaphores
	
	return 0;
}

int insert_item(buffer_item item){
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