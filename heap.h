#ifndef HEAP_HEADER
#define HEAP_HEADER

#define SIZE_HEAP 128
#define FREE_BLOCK -1


int libre;

char heap[SIZE_HEAP];

void init_heap(void);
// allocate a pointer of size size
char *heap_malloc(unsigned int size);
// free a pointer in the heap
int heap_free(char *ptr);

#endif