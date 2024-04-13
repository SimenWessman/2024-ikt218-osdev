// ------------------
// |     gdt.h      |
// ------------------

#ifndef GDT_H
#define GDT_H
#include "libc/stdint.h"

// -----------------------
//     GDT CONSTANTS     |
// -----------------------
// Segment type definitions
#define GDT_CODE_EXEC_READ 0x9A  // 10011010b: Executable, Readable, Accessed bit off, Privilege level 0
#define GDT_DATA_READ_WRITE 0x92 // 10010010b: Not Executable, Read/Write, Accessed bit off, Privilege level 0

// DPL (Descriptor Privilege Level) flags
#define GDT_FLAG_RING0 0x00 // Ring 0
#define GDT_FLAG_RING3 0x60 // Ring 3 (01100000b: sets the DPL to Ring 3)

#define GDT_GRANULARITY_4K 0x80 // 10000000b: Specifies granularity of 4KB
#define GDT_32_BIT_MODE 0x40    // 01000000b: Specifies 32-bit opcodes

// -----------------------
//     GDT FUNCTIONS     |
// -----------------------

// Function prototypes
extern void load_gdt(void);

// GDT entry structure
struct gdt_entry 
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr 
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// Function declarations
void init_gdt();

// Function to create a GDT entry
struct gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

#endif
