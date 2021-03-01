Memory Allocation Subroutine
======

This package includes memory memory allocation (and deallocation) subroutines I made for a class project. I use a Linked List data structure to keep track of free chunks that can be used for memory allocation and use system call sbrk() and pointer arithmetic to properly allocate memory in the heap space. There are four main methods I define:

* my_malloc(): allocates memory of specified size (in bytes). returns a pointer to the memory.
* my_free(): frees memory of the object referenced by the pointer. adds freed memory to the free list.
* free_list_begin(): returns the pointer to the head of the free list.
* coalesce_free_list(): goes through the list to merge adjacent free nodes.

Additional helper methods are defined in the header file.

