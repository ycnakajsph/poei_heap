#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../heap.h"

void test_heap_init(){
	init_heap();

	CU_ASSERT(heap[0] == SIZE_HEAP-1);
	CU_ASSERT(heap[1] == FREE_BLOCK);
	CU_ASSERT(libre == 0);
}

void test_first_fit_init(){
	init_heap();

	char* ptr = first_fit(10);

	CU_ASSERT(ptr == heap); // in case the heap is empty the first fit is the heap addr

	ptr = first_fit(SIZE_HEAP);

	CU_ASSERT(ptr == NULL); // in case we asks more than the heap size

	ptr = first_fit(2*SIZE_HEAP);

	CU_ASSERT(ptr == NULL); // in case we asks more than the heap size

	ptr = first_fit(SIZE_HEAP-1);

	CU_ASSERT(ptr == heap); // in case we asks more than the heap size

}

void test_first_fit_simple(){
	init_heap();

	heap[0] = 3;
	heap[1] = 'a';
	heap[2] = 'b';
	heap[3] = '\0';
	heap[4] = 123;
	heap[5] = -1;

	char* ptr = first_fit(10);

	CU_ASSERT(ptr == heap+4);

	ptr = first_fit(123);

	CU_ASSERT(ptr == heap+4);

	ptr = first_fit(124);

	CU_ASSERT(ptr == NULL);

}

void test_first_fit_more(){
	init_heap();

	heap[0] = 10;
	strcpy(heap+1,"tp1");

	heap[0+10+1] = 9;
	strcpy(heap+11+1,"tp2");

	heap[11+9+1] = 5;
	strcpy(heap+21+1,"tp3");

	heap[27] = 100;
	heap[28] = -1;

	libre = 27;

	char* ptr = first_fit(10);

	CU_ASSERT(ptr == heap+27);
}

void test_first_fit_dont_fit_in_between(){
	init_heap();

	heap[0] = 10;
	strcpy(heap+1,"tp1");

	heap[0+10+1] = 9;
	heap[11+1] = -1;

	libre = 11;

	heap[11+9+1] = 5;
	strcpy(heap+21+1,"tp3");

	heap[27] = 100;
	heap[28] = -1;

	char* ptr = first_fit(10);

	CU_ASSERT(ptr == heap+27);
}

void test_first_fit_fit_in_between(){
	init_heap();

	heap[0] = 10;
	strcpy(heap+1,"tp1");

	heap[0+10+1] = 9;
	heap[11+1] = -1;

	libre = 11;

	heap[11+9+1] = 5;
	strcpy(heap+21+1,"tp3");

	heap[27] = 100;
	heap[28] = -1;

	char* ptr = first_fit(9);

	CU_ASSERT(ptr == heap+11);
}

void test_find_new_libre(){
	int future_libre;

	init_heap();

	heap[0] = 10;
	strcpy(heap+1,"tp1");

	heap[0+10+1] = 9;
	heap[11+1] = -1;

	libre = 11;

	heap[11+9+1] = 5;
	strcpy(heap+21+1,"tp3");

	heap[27] = 100;
	heap[28] = -1;

	heap[12] = INIT_VAL;

	future_libre = find_new_libre(heap+11);

	CU_ASSERT(future_libre == 27);
}

void test_heap_malloc_example(){

	init_heap();

	char* p1 = heap_malloc(10);
	/*print_heap();*/

	CU_ASSERT(p1-1 == heap);
	CU_ASSERT(*(p1+1) != FREE_BLOCK);

	CU_ASSERT(libre == 10+1);
	CU_ASSERT(*(heap + libre) == 116);
	CU_ASSERT(*(heap + libre + 1) == FREE_BLOCK);

	char* p2 = heap_malloc(9);
	/*print_heap();*/

	CU_ASSERT(p2 == heap+12);
	CU_ASSERT(*(p2-1) == 9);
	CU_ASSERT(libre == 21);

	char* p3 = heap_malloc(5);
	/*print_heap();*/

	CU_ASSERT(p3 == heap + 22);
	CU_ASSERT(*(p3-1) == 5);
	CU_ASSERT(libre == 27);

	char* p4 = heap_malloc(101);
	/*print_heap();*/

	CU_ASSERT(p4 == NULL);
}

void test_heap_free(){

	init_heap();

	char* p1 = heap_malloc(10);
	
	heap_free(p1);

	CU_ASSERT(heap[0] == 127);
	CU_ASSERT(heap[1] == FREE_BLOCK);

}

void test_heap_free_several(){

	init_heap();

	char* p1 = heap_malloc(10);
	char* p2 = heap_malloc(10);
	char* p3 = heap_malloc(10);
	char* p4 = heap_malloc(10);

	strcpy(p1,"tp1");
	strcpy(p2,"tp2");
	strcpy(p3,"tp3");
	strcpy(p4,"tp4");
	
	heap_free(p2); // simple free

	CU_ASSERT(*(p2-1) == 10);
	CU_ASSERT(*(p2) == FREE_BLOCK);
	CU_ASSERT(libre == p2 - 1 - heap);

	heap_free(p3); // testing merge left

	CU_ASSERT(*(p3) == INIT_VAL);
	CU_ASSERT(*(p3-1) == INIT_VAL);

	CU_ASSERT(*(p2-1) == 21);
	CU_ASSERT(*(p2) == FREE_BLOCK);
	CU_ASSERT(libre == p2 - 1 - heap);

	heap_free(p1); // testing merge right

	CU_ASSERT(*(p1-1) == 32);
	CU_ASSERT(*(p1) == FREE_BLOCK);
	CU_ASSERT(libre == 0);

}

void test_full_example(){
	init_heap();

	char* p1 = heap_malloc(10);
	char* p2 = heap_malloc(9);
	char* p3 = heap_malloc(5);

	strcpy(p1,"tp 1");
	strcpy(p2,"tp 2");
	strcpy(p3,"tp 3");

	heap_free(p2);

	// That verifies that in case we have 1 byte left we actually malloc size+1
	char* p4 = heap_malloc(8);
	strcpy(p4,"systeme");

	/*print_heap();*/

	CU_ASSERT(*(p1-1) == 10);
	CU_ASSERT(*(p4-1) == 9);
	CU_ASSERT(*(p3-1) == 5);
	CU_ASSERT(libre == 27);
	CU_ASSERT(heap[27] == 100);
	CU_ASSERT(heap[28] == -1);

	CU_ASSERT(strcmp(p1,"tp 1") == 0);
	CU_ASSERT(strcmp(p3,"tp 3") == 0);
	CU_ASSERT(strcmp(p4,"systeme") == 0);
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		NULL == CU_add_test(pSuite, "test_heap_init()", test_heap_init)||
		NULL == CU_add_test(pSuite, "test_first_fit_init()", test_first_fit_init)||
		NULL == CU_add_test(pSuite, "test_first_fit_simple()", test_first_fit_simple)||
		NULL == CU_add_test(pSuite, "test_first_fit_more()", test_first_fit_more)||
		NULL == CU_add_test(pSuite, "test_first_fit_fit_in_between()", test_first_fit_fit_in_between)||
		NULL == CU_add_test(pSuite, "test_first_fit_dont_fit_in_between()", test_first_fit_dont_fit_in_between)||
		NULL == CU_add_test(pSuite, "test_find_new_libre()", test_find_new_libre)||
		NULL == CU_add_test(pSuite, "test_heap_malloc_example()", test_heap_malloc_example)||
		NULL == CU_add_test(pSuite, "test_heap_free()", test_heap_free)||
		NULL == CU_add_test(pSuite, "test_heap_free_several()", test_heap_free_several)||
		NULL == CU_add_test(pSuite, "test_full_example()", test_full_example)
	)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
