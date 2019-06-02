#ifndef _MEM_H_
#define _MEM_H_
#define _USE_MISC

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdbool.h>

#define HEAP_START ((void*)0x04040000)
#define DEBUG_FIRST_BYTES 4
#define BLOCK_MIN_SIZE 1024 // minimal 50 bytes 
#define PAGE_SIZE BLOCK_MIN_SIZE * 4
struct mem;
#pragma pack(push, 1)
struct mem {
	struct mem* next;
	size_t capacity;
	bool is_free;
};
#pragma pack(pop)

#define BLOCK_SIZE sizeof(struct mem)
////////////////void* firstll = NULL; // o primeiro bloco 

void* _malloc(size_t query);
void _free(void* mem);
void* heap_init(size_t initial_size);
void* new_space(void* addr, size_t totalsize);
void trysplit(struct mem* blk, size_t query);

struct mem* new_block(struct mem* last, size_t query);
struct mem* find_block(struct mem* block, size_t query);
struct mem* re_lastblock(struct mem* first);


void memalloc_debug_struct_info( FILE * f,
		struct mem const* const address);

void memalloc_debug_heap( FILE * f, struct mem const* ptr);


#endif

