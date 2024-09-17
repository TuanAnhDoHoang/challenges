#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include"heap.h"
#include"header.h"

uint8_t *datas[MAX_DATA];
uint64_t sizes[MAX_DATA];

void setup()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

uint16_t get_choice()
{
    uint8_t inline_buf[0x10];
    uint16_t n;
    puts("1. Create data");
    puts("2. Read data");
    puts("3. Delete data");
    printf(">> ");
    n = read(0, inline_buf, 0x10);
    if(inline_buf[n - 1] == '\n')
        inline_buf[n - 1] = '\0';
    return atoi(inline_buf);
}

void add_data()
{
    uint64_t size, index;
    uint8_t *tmp;

    printf("Index: ");
    scanf("%lld", &index);
    printf("Size: ");
    scanf("%lld", &size);

    tmp = get_chunk(size);
    printf("Data: ");
    read(0, tmp, size);
    
    datas[index] = tmp;
    sizes[index] = size;
    puts("Done!");
}

void delete_data()
{
    uint64_t index;

    printf("Index: ");
    scanf("%lld", &index);

    free_chunk(datas[index]);
    datas[index] = NULL;
    sizes[index] = 0;

    puts("Done!");

}

void view_data()
{
    uint64_t index;

    printf("Index: ");
    scanf("%lld", &index);

    printf("Your data: ");
    write(1, datas[index], sizes[index]);
}
int main()
{
    puts(FAI);
    puts("Vulnerable program that doesn't check anything");
    setup();
    init_heaps();
    while(1)
    {
        uint16_t choice;
        choice = get_choice();
        switch (choice)
        {
        case 1:
            add_data();
            break;
        case 2:
            view_data();
            break;
        case 3:
            delete_data();
            break;
        default:
            break;
        }

    }   
    return 0;
}
