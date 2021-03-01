
#ifndef my_malloc_h
#define my_malloc_h

#include <stdint.h>

//header size for allocated chunks is 8.
//4 bytes for the size of the whole allocated chunk (header+data+padding). 4 bytes for magic number to verify the chunk is allocated.
#define CHUNKHEADERSIZE 8


//I use a linked list for my free list implementation
typedef struct freenode {
    struct freenode *flink;
    uint32_t size;
} FreeNode;

//my_malloc(): allocates memory of specified size (in bytes). returns a pointer to the memory.
void *my_malloc(uint32_t size);

//my_free(): frees memory of the object referenced by the pointer. adds freed memory to the free list.
void my_free(void *ptr);

//free_list_begin(): returns the head of the free list.
FreeNode *free_list_begin(void);

//coalesce_free_list(): goes through the list to merge adjacent free nodes
void coalesce_free_list(void);

//my_malloc_heap(): called by my_malloc. allocates memory from newly extended heap space. returns pointer.
void *my_malloc_heap(uint32_t);

//split_chunk(): allocates only needed amount of memory out of the free space chunk. the remainder is added to the free list through add_free_node(). returns the size of the allocated chunk.
uint32_t split_chunk(void *, uint32_t, uint32_t);

//add_free_node(): adds deallocated memory or extra space from split_chunk() to the free list. order is based on memory location.
void add_free_node(FreeNode *, FreeNode *, uint32_t);

//find_free_chunk(): looks for a free node of sufficient size for memory allocation. returns the first appropriate free node. returns null if no node is found.
FreeNode * find_free_chunk(uint32_t);

//my_malloc_node(): called by my_malloc. allocates memory from the specified node from the free list. returns pointer.
void *my_malloc_node(uint32_t, FreeNode *);

#endif
