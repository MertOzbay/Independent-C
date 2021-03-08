
#include "men_alloc.h"
#include <stdlib.h>
#include <unistd.h>

static FreeNode * head;

oid *my_malloc(uint32_t size)
{
  if (size == 0) return NULL;
  size += CHUNKHEADERSIZE;
  if (size%8 != 0) size += (8-(size%8));

  if (head==0)
    return my_malloc_heap(size);

  FreeNode * chunk = find_free_chunk(size);
  if (chunk==0) return my_malloc_heap(size);

  return my_malloc_node(size, chunk);
}
void my_free(void *ptr)
{
  if (ptr == 0 ) {
    printf("ERROR: cannot free a null pointer\n");
    exit(1);
    return;
  }

  uint32_t mag_num = 99999999;
  if (*((uint32_t *)ptr-1) == mag_num) {
    uint32_t size = *(((uint32_t *)ptr)-2);
    *(((uint32_t *)ptr)-2) = 0;
    ptr = (uint32_t *)ptr-2;
    FreeNode * node = (FreeNode *) ptr;
    add_free_node(node, size);
  } else {
    printf("ERROR: trying to free non-allocated chunk\n");
    exit(1);
  }

}

FreeNode * free_list_begin()
{
  return head;
}

void coalesce_free_list()
{
  FreeNode * curr = head;
  if (head == 0) return;
  FreeNode * next = head -> flink;
  while(next != 0){
    uint32_t size = curr->size;
    FreeNode * try = (FreeNode *)((char*)curr+size);

    if (try == next){
      curr->size = size + next->size;
      curr->flink = next -> flink;
      continue;
    }

    curr = next;
    next = next -> flink;
  }
}

void *my_malloc_heap(uint32_t size) {
  void * ptr = sbrk(0);
  uint32_t sbrk_size = 8192;
  uint32_t mag_num = 99999999;

  if (size>sbrk_size)
  {
    sbrk_size = size;
    uint32_t sbrk_result = sbrk(sbrk_size);
    if (sbrk_result<0) {
      printf("ERROR: no memory available!\n");
      exit(1);
    }
  }
  else
  {
    uint32_t sbrk_result = sbrk(sbrk_size);
    if (sbrk_result<0) {
      printf("ERROR: no memory available!\n");
      exit(1);
    }

    size = split_chunk(ptr, sbrk_size, size);
  }
  //ptr = (uint32_t *) ptr;
  *  (uint32_t *) ptr = size;
  *  (uint32_t *)(ptr + 4) = mag_num;
  return (void *) (ptr + 8);
}

uint32_t split_chunk(void *ptr, uint32_t chunk_size, uint32_t alloc_size){
  uint32_t free_node_size = 0;

  if (chunk_size >= alloc_size*2){
    free_node_size = (chunk_size-alloc_size);
  }

  FreeNode * free_node_begin = (FreeNode *) (ptr + alloc_size);


  if (free_node_size > 15){
    add_free_node(free_node_begin, free_node_size);
  }

  return alloc_size;
}

void add_free_node(FreeNode * free_node_begin, uint32_t free_node_size) {

  free_node_begin->size = free_node_size;

  if (head == 0){
    head = free_node_begin;
    head -> flink = 0;
  }
  else
  {
    if (head>free_node_begin){
      free_node_begin ->flink = head;
      head = free_node_begin;
    } else {
      FreeNode * curr = head;
      FreeNode * next = head -> flink;
      while (curr<free_node_begin && next != 0){
        curr = next;
        next = curr -> flink;
      }
      curr -> flink = free_node_begin;
      free_node_begin -> flink = next;
    }
  }

}

FreeNode * find_free_chunk(uint32_t size){
  FreeNode * prev = head;
  FreeNode * curr = prev -> flink;
  while(curr != 0){
    if (curr->size >= size ) {
      prev ->flink = curr -> flink;
      return curr;
    }
    else {
      prev = curr;
      curr = curr -> flink;
    }
  }
  return curr;
}

void *my_malloc_node(uint32_t size, FreeNode * chunk){
  void * ptr;
  uint32_t mag_num = 99999999;

  uint32_t free_chunk_size = chunk -> size;

  size = split_chunk(ptr, free_chunk_size, size);

  *  (uint32_t *) ptr = size;
  *  (uint32_t *)(ptr + 4) = mag_num;
  return (void *) (ptr + 8);
}
