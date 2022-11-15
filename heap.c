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

void merge_free_blocks(char* block_left, char* block_right){
	char block_l_size = *block_left;
	char block_r_size = *block_right;

	*block_right = INIT_VAL;
	*(block_right+1) = INIT_VAL;

	*block_left = block_l_size + block_r_size + 1;
}

void right_merge_free_blocks(char* ptr){
	char size_block = *(ptr-1);
	char* next_block;
	char val_next_block;

	next_block = ptr+size_block;
	val_next_block = *(next_block+1);

	if ( val_next_block == FREE_BLOCK){ // it means we have to merge the blocks
		merge_free_blocks(ptr-1,next_block);
	}
}

void left_merge_free_blocks(char* ptr){
	char* ptr_left;
	char size_left_free_block;

	ptr_left = ptr-1;
	while(*ptr_left != FREE_BLOCK && ptr_left > heap){ // rewind to the first FREE_BLOCK
		ptr_left--;
	}

	if(ptr_left > heap){
		size_left_free_block = *(ptr_left-1);
		if (ptr_left + size_left_free_block + 1 == ptr){
			// We shall merge the blocks, the first free block on the left is glued to this one
			merge_free_blocks(ptr_left-1,ptr-1);
		}
	}
}

void heap_free(char* ptr){ // We'll assume that ptr is a correct pointer

	*(ptr) = FREE_BLOCK;

	right_merge_free_blocks(ptr);

	// Now we need to look left to see if a block might be merged
	left_merge_free_blocks(ptr);
}

char *heap_malloc(unsigned int size){

	char* ptr = first_fit(size);
	char leftover;

	if (ptr == NULL){
		return NULL;
	}
	*(ptr+1) = INIT_VAL; // Removing FREE_BLOCK value

	leftover = *ptr - size;
	/*printf("leftover == %d\n",leftover);*/
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
		/*printf("updating libre to %d\n", libre);*/
	}

	return ptr+1;
}
