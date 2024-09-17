

#ifndef HEAP_H
#define HEAP_H
#include"debug.h"
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

#define HEAP_ALIGN 0x10
#define NUM_HEAP 0x8
#define MAX_SIZE 0x1000
extern uint16_t Heap_sizes[NUM_HEAP];
extern uint16_t Heap_max[NUM_HEAP];
typedef struct free_node
{
    struct free_node *next;
    
}free_node;
typedef struct Heap
{
    uint16_t chunk_size;
    free_node *freelist;
    uint16_t max_count;
    uint16_t count;
    void *start;
    void *end;
}Heap;

extern Heap heaps[NUM_HEAP];
void* get_chunk(size_t size);
void free_chunk(void* chunk);
void push_freelist(Heap *heap_, free_node *chunk);
void* pop_freelist(Heap *heap_);
uint16_t get_idx_from_chunk(Heap *heap_, void *chunk);
uint16_t get_heap_from_size(size_t size);
uint16_t find_heap_by_chunk(void *chunk);

static inline void check_valid(int value, char *log)
{
    if(!value)
        fail(log);
}


#endif