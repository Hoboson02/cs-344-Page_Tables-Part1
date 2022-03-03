#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MEM_SIZE 16384  // MUST equal PAGE_SIZE * PAGE_COUNT
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define PAGE_COUNT 64
#define PAGE_SHIFT 8  // Shift page number this much
// Simulated RAM
unsigned char mem[MEM_SIZE];

unsigned char get_page_table(int proc_num)
{
    return mem[proc_num + 64];
}