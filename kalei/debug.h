#ifndef DEBUG_H
#define DEBUG_H
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static inline void fail(char *log)
{
    puts(log);
    exit(-1);
}
char* get_hexlify(const char* bytes, const size_t numBytes);
//#define DEBUG 

#endif