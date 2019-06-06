#include "mem.h"

void* _calloc(size_t nitems, size_t size){
	size_t totalsize = nitems*size;
	size_t count;
	void* bf = _malloc(totalsize);
	
	for(count=0; count<totalsize; count++) 
		((char*)bf)[count] = 0;
	return bf;
}
void* _malloc(size_t query){
	size_t allocsize = query >= BLOCK_MIN_SIZE ? query : BLOCK_MIN_SIZE;
	struct mem* block = find_block(HEAP_START, query);
	if(block == NULL){
		// requisitar mais memoria
		struct mem* last = re_lastblock(HEAP_START);
		block = new_block(last, allocsize); // retorna o endereço do bloco inteiro 
		trysplit(block, query);
	}
	else{
		trysplit(block, query);
		block->is_free = false;
	}
	return ((char*)block + BLOCK_SIZE);	
}

void _free(void* blk){
	if(blk == NULL) return;
	struct mem* block = (struct mem*)( (char*)blk - BLOCK_SIZE ) ;
	struct mem* posblk = block->next;
	block->is_free = true;
	
	if(posblk != NULL){
		if(posblk->is_free == true){
			block->capacity += posblk->capacity;
			block->next = posblk->next;

		}
	}
}

void trysplit(struct mem* blk, size_t query){
	struct mem* newblock;
	size_t blksize = blk->capacity - query - BLOCK_SIZE;// MUDANÇA
	size_t totalblocksize = query + BLOCK_SIZE;
	if(blksize <= 0)
		return;
	newblock = (struct mem*)((char *)blk + totalblocksize);
	newblock->capacity = blksize;
	newblock->next = blk->next;
	newblock->is_free = true;
	blk->next = newblock;
	blk->capacity = query;

}
struct mem* re_lastblock(struct mem* first){
	struct mem* last;
	for(last=first; last->next != NULL ; last=last->next);
	return last;

}
struct mem* new_block(struct mem* last, size_t query){
	struct mem* block = NULL;
	struct mem* addr = (struct mem*)last + sizeof(last) + last->capacity ;  // tamanho total do bloco 
	size_t totalsize = query * 2; 
	block = (struct mem*) new_space(addr, totalsize);

	if(block == MAP_FAILED){
		block = (struct mem*) new_space(NULL, totalsize);
		if(block == MAP_FAILED)
			return NULL; // erro 
	}

	if(last != NULL)
		last->next = block;

	block->capacity = totalsize;
	block->next = NULL;	
	block->is_free = false;
	return block;

}

void* new_space(void* addr, size_t totalsize){
	void* new_addr = mmap(addr, totalsize, PROT_READ | PROT_WRITE | PROT_EXEC, 
			MAP_PRIVATE | MAP_ANON, -1, 0);
	return new_addr;
}
struct mem* find_block(struct mem* block, size_t query){
	for(;block; block=block->next){
		if( block->is_free && block->capacity >= query )
			return block;
	}	

	return NULL;
}

void* heap_init(size_t initial_size){
	size_t allocsize = initial_size >= BLOCK_MIN_SIZE ? initial_size+BLOCK_SIZE : BLOCK_MIN_SIZE;
	mmap( HEAP_START, allocsize,
		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_ANON | MAP_PRIVATE, -1, 0);
	struct mem *base = HEAP_START;
	base->next = NULL;
	base->capacity = allocsize - BLOCK_SIZE;
	base->is_free = true;

	return base;
}

void memalloc_debug_struct_info(FILE* f, struct mem const* const address) {
    size_t i;
    fprintf(f, "start: %p\nsize: %lu\nis_free: %d\n", (void*) address, address->capacity, address->is_free);
    for ( i = 0; i <  DEBUG_FIRST_BYTES  &&  i <  address-> capacity; ++i )
        fprintf( f, "%hhX", ((char*)address)[ sizeof( struct mem ) + i ] );
    putc( '\n', f );
}

void memalloc_debug_heap( FILE* f, struct mem const* ptr ) {
    for(; ptr; ptr = ptr->next)
        memalloc_debug_struct_info( f, ptr );
    printf("================\n");
}
