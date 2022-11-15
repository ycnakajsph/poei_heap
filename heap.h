#ifndef HEAP_HEADER
#define HEAP_HEADER

#define SIZE_HEAP 128
#define FREE_BLOCK -1
#define INIT_VAL 0
#define MEMORY_FULL -1

int libre;

char heap[SIZE_HEAP];

void init_heap(void);
// allocate a pointer of size size
char *heap_malloc(unsigned int size);
// free a pointer in the heap
int heap_free(char *ptr);

char* first_fit(unsigned int size);
int find_new_libre(char* ptr);

void print_heap(void);

#endif