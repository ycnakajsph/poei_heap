#include <stdio.h>
#include "heap.h"

void init_heap(void){
	heap[0] = SIZE_HEAP-1;
	heap[1] = FREE_BLOCK;
	libre = 0;
}
