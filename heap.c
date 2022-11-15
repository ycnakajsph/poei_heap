#include <stdio.h>
#include <ctype.h>
#include "heap.h"

void print_heap(void){
 printf ("libre = %d\n", libre);
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 16; j++) { 
      printf("%4d", j + 16*i);
    }
    printf("\n");
    for (j = 0; j < 16; j++) { 
      printf("%4d", heap[j + 16*i]);
    }
    printf("\n");
    for (j = 0; j < 16; j++) { 
      if (isprint(heap[j + 16*i])) {
	printf("%4c", heap[j + 16*i]);
      } else {
	printf("    ");
      }
    }
    printf("\n\n");
  }
  printf("---------------------------------------------------------------\n\n");
}

void init_heap(void){
	int i;

	for(i=0;i<SIZE_HEAP;i++){ // Might be test only at some point
		heap[i] = INIT_VAL;
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

int find_new_libre(char* ptr){
	int jump;

	/*jump = *ptr+1;
	ptr = ptr + jump; // move to the next block*/

	while (ptr < heap+SIZE_HEAP-1){ // Walks through the heap
		if (*(ptr+1) == FREE_BLOCK){ // if found a free block return its index
			return ptr-heap;
		}
		jump = *ptr+1;
		ptr = ptr + jump;
	}
	return MEMORY_FULL;
}


char *heap_malloc(unsigned int size){

	char* ptr = first_fit(size);
	char leftover;

	if (ptr == NULL){
		return NULL;
	}
	*(ptr+1) = INIT_VAL; // Removing FREE_BLOCK value

	leftover = *ptr - size;
	printf("leftover == %d\n",leftover);
	if(leftover > 1){
		*ptr = size;
		*(ptr+size+1) = leftover-1;
		*(ptr+size+2) = FREE_BLOCK;
	}
	else if (leftover == 0){
		*ptr = size;
	}
	else {
		*ptr = size + 1; // in case there's only 1 byte left we allocate it too
	}

	if (ptr-heap == libre){ // we need to update libre then
		libre = find_new_libre(ptr);
		printf("updating libre to %d\n", libre);
	}

	return ptr+1;
}
