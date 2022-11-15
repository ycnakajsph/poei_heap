#ifndef HEAP_HEADER
#define HEAP_HEADER

// allocate a pointer of size size
char *heap_malloc(unsigned int size);
// free a pointer in the heap
int heap_free(char *ptr);

#endif