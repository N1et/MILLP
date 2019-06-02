#include <stdio.h>
#include "lib/mem.h"

int main() {
    heap_init(1000);
	memalloc_debug_heap(stdout, HEAP_START);
    void* mem1 = _malloc(111);
    void* mem2 = _malloc(333);
    void* mem3 = _malloc(555);
    void* mem4 = _malloc(666);

	memalloc_debug_heap(stdout, HEAP_START);
    _free(mem2);
    memalloc_debug_heap(stdout, HEAP_START);
    _free(mem3);

	 *(int*)mem4 = 0x555;
	 *(int*)mem1 = 0x5050;
    memalloc_debug_heap(stdout, HEAP_START);

    return 0;
}
