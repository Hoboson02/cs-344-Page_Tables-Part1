// Name: Ryan Earl
// Project 9: Page Tables Part 1

#include "ptsim.h"
//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset;
}
//
// Initialize RAM
//
void initialize_mem(void)
{
    // TODO
    mem[MEM_SIZE] = 0;// 1. Zero every byte of physical memory in the mem array.
    mem[0] = 1; // 2. Mark zero page as "used" in the Used Page Table. (That is, set mem[0] to 1.)
}
//
// Allocate a physical page
//
// Returns the number of the page, or 0xff if no more pages available
//
unsigned char get_page(void)
{
    // TODO
    for (int page_number = 0; page_number <= PAGE_COUNT; page_number ++) {  //For each page_number in the Used Page array(Used Page array is the first 64 bytes of zero page) in zero page:
    	if (mem[page_number] == 0) {	  //     If it's unused (if it's 0):
            mem[page_number] = 1; // mark used
    		return page_number;//         return the page_number
		}
	}

    return 0xff;  // indicating no free pages
}
//
// Allocate pages for a new process
//
// This includes the new process page table and page_count data pages.
//
void new_process(int proc_num, int page_count)
{
    // TODO
    // Get the page table page
    int page_table = get_page();

    // Set this process's page table pointer in zero page
    mem[64 + proc_num] = page_table;

    // Allocate data pages
    for (int i = 0; i < page_count; i++) { // For i from 0 to page_count:
        int new_page = get_page();

        // page_table = mem[i]->new_page;// Set the page table to map virt -> phys
        // Virtual page number is i
        // Physical page number is new_page
        int pt_addr = get_address(page_table, i);
        mem[pt_addr] = new_page;
	}
}
//
// Print the free page map
//
void print_page_free_map(void)
{
    printf("--- PAGE FREE MAP ---\n");
    for (int i = 0; i < 64; i++) {
        int addr = get_address(0, i);
        printf("%c", mem[addr] == 0? '.': '#');
        if ((i + 1) % 16 == 0)
            putchar('\n');
    }
}
//
// Print the address map from virtual pages to physical
//
void print_page_table(int proc_num)
{
    printf("--- PROCESS %d PAGE TABLE ---\n", proc_num);
    // Get the page table for this process
    int page_table = get_page_table(proc_num);
    // Loop through, printing out used pointers
    for (int i = 0; i < PAGE_COUNT; i++) {
        int addr = get_address(page_table, i);
        int page = mem[addr];
        if (page != 0) {
            printf("%02x -> %02x\n", i, page);
        }
    }
}
//
// Main -- process command line
//
int main(int argc, char *argv[])
{
    assert(PAGE_COUNT * PAGE_SIZE == MEM_SIZE);
    if (argc == 1) {
        fprintf(stderr, "usage: ptsim commands\n");
        return 1;
    }
    
    initialize_mem();
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "np") == 0) {
            int proc_num = atoi(argv[++i]);
            int pages = atoi(argv[++i]);
            new_process(proc_num, pages);
        }
        else if (strcmp(argv[i], "pfm") == 0) {
            print_page_free_map();
        }
        else if (strcmp(argv[i], "ppt") == 0) {
            int proc_num = atoi(argv[++i]);
            print_page_table(proc_num);
        }
    }
}
