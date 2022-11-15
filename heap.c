#include <stdio.h>
#include "heap.h"

void init_heap(void){
	int i;

	for(i=0;i<SIZE_HEAP;i++){ // Might be test only at some point
		heap[i] = 0;
	}

	heap[0] = SIZE_HEAP-1;
	heap[1] = FREE_BLOCK;
	libre = 0;
}

char* first_fit(unsigned int size){

	int jump;
	char* ptr = heap+libre;
	if (*ptr >= size){
		return ptr;
	}
	while(ptr < heap+SIZE_HEAP-1){ // Walks through the heap
		// if the current block is free and has the proper size
		if (*ptr >= size && *(ptr+1) == FREE_BLOCK){
			return ptr;
		}
		jump = *ptr+1;
		ptr = ptr + jump;
		/*printf("moving to ptr + %d\n", jump);*/
	}
	return NULL;
}

/*
char *heap_malloc(unsigned int size){

	char* ptr = first_fit(size);

}*/