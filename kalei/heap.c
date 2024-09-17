#include"heap.h"
#define __USE_MISC  
#include <sys/mman.h>
#include <fcntl.h>

#include"debug.h"
uint16_t Heap_sizes[NUM_HEAP] = {0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x1000};
uint16_t Heap_max[NUM_HEAP] = {0x40, 0x40, 0x40, 0x30, 0x30, 0x20, 0x20, 0x10};
Heap heaps[NUM_HEAP];

uint64_t get_rand()
{
    int fd = open("/dev/urandom", O_RDONLY);
    check_valid(fd > 0, "Open random failed");
    uint64_t high, low, addr;
    read(fd, &high, 8);
    read(fd, &low, 8);

    addr = (high << 32) ^ low;
    addr &= 0x7ffffffff000; 

    close(fd);
    return addr;
}

void init_heaps()
{
    for(int i = 0;i < NUM_HEAP; i++)
    {
        heaps[i].start = mmap(get_rand(), Heap_sizes[i]*Heap_max[i], PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        check_valid(heaps[i].start != NULL, "Initialize heaps fail!");
        heaps[i].chunk_size = Heap_sizes[i];
        heaps[i].count = 0;
        heaps[i].end = heaps[i].start + Heap_sizes[i]*Heap_max[i];
        heaps[i].freelist = NULL;
        heaps[i].max_count = Heap_max[i];
    }
}

uint16_t get_idx_from_chunk(Heap *heap_, void *chunk)
{
    check_valid(((uint64_t)chunk & 0xf) == 0, "Chunk is not aligned!");
    return ((uint64_t)chunk - (uint64_t)heap_->start) / heap_->chunk_size;
}

uint16_t get_heap_from_size(size_t size)
{
    for(uint16_t i = 0; i < NUM_HEAP; i++)
        if(Heap_sizes[i] >= size)
            return i;
}

void push_freelist(Heap *heap_, free_node *chunk)
{
    check_valid(((uint64_t)chunk % HEAP_ALIGN) == 0, "Chunk is not aligned!");
    if(heap_->freelist)
    {
        chunk->next = heap_->freelist;
        heap_->freelist = chunk;
    }
    else
    {
        heap_->freelist = chunk;
        chunk->next = NULL;
    }
}

void* pop_freelist(Heap *heap_)
{
    if(!heap_->freelist)
        return 0;
    void *chunk;
    chunk = heap_->freelist;
    heap_->freelist = heap_->freelist->next;
    return chunk;
}

void* get_chunk(size_t size)
{
    uint16_t heap_idx = get_heap_from_size(size);
#ifdef DEBUG
    printf("Input size: %d\n", size);
    printf("Heap index: %d\n", heap_idx);
    printf("Heap size: %d\n", heaps[heap_idx].chunk_size);
#endif
    void *chunk;

    do
    {
        chunk = pop_freelist(&heaps[heap_idx]);
        if(chunk)
            return chunk;
        if(heaps[heap_idx].count >= heaps[heap_idx].max_count)
        {
            heap_idx++;
            continue;
        }
        chunk = heaps[heap_idx].start + heaps[heap_idx].chunk_size*(heaps[heap_idx].count);
        heaps[heap_idx].count++;
        break;
    }while(heap_idx < NUM_HEAP);
#ifdef DEBUG
    printf("Allocate %d bytes: %p\n", size, chunk);
    printf("Chunk size: %d\n", heaps[heap_idx].chunk_size);
#endif
    return chunk; 
}

uint16_t find_heap_by_chunk(void *chunk)
{
    for(uint16_t i = 0; i < NUM_HEAP; i++)
        if(chunk >= heaps[i].start && chunk < heaps[i].end)
            return i;
    return -1;
}

void free_chunk(void* chunk)
{
    if(!chunk)
        return;
    uint16_t heap_idx = find_heap_by_chunk(chunk);
    check_valid(heap_idx < NUM_HEAP, "Free invalid chunk");
    push_freelist(&heaps[heap_idx], (free_node*)chunk);
#ifdef DEBUG
    printf("Put %p chunk to heap %d size\n", chunk, heaps[heap_idx].chunk_size);
#endif
}