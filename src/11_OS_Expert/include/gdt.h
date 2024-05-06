// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/gdt.h
// 
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// Header file for the Global Descriptor Table (GDT) management. This
// file defines structures and constants necessary for setting up
// the GDT, which is essential for defining different memory segments
// in protected mode.
// -----------------------------------------------------------------------------

#ifndef GDT_H
#define GDT_H

#include "libc/stdint.h"  // Include for fixed-width integer types.

// -----------------------
// GDT Constants
// -----------------------
// Constants for segment type definitions. These determine the characteristics
// and privileges of the segments defined in the GDT.

// Executable and readable code segment, not accessed, with Ring 0 privilege.
#define GDT_CODE_EXEC_READ 0x9A  // Binary: 10011010

// Readable and writable data segment, not accessed, with Ring 0 privilege.
#define GDT_DATA_READ_WRITE 0x92 // Binary: 10010010

// Descriptor Privilege Level (DPL) flags.
#define GDT_FLAG_RING0 0x00      // Ring 0: Highest level of privilege.
#define GDT_FLAG_RING3 0x60      // Ring 3: Lowest level, used for user-space applications.

// Granularity and operation size flags.
#define GDT_GRANULARITY_4K 0x80  // Granularity: 1 = 4KB, 0 = 1 byte.
#define GDT_32_BIT_MODE 0x40     // 32-bit opcode size.

// -----------------------
// GDT Structures
// -----------------------
// These structures represent the format of GDT entries and the GDT register
// (GDTR) format used by the lgdt instruction.

// Structure defining a single GDT entry.
struct gdt_entry {
    unsigned short limit_low;     // Lower 16 bits of the segment limit.
    unsigned short base_low;      // Lower 16 bits of the base address.
    unsigned char base_middle;    // Next 8 bits of the base address.
    unsigned char access;         // Access flags (type, privilege level, present).
    unsigned char granularity;    // Granularity, size, and some limit bits.
    unsigned char base_high;      // Last 8 bits of the base address.
} __attribute__((packed));        // The packed attribute ensures no padding is added.

// Structure for the GDTR register, loaded using the lgdt instruction.
struct gdt_ptr {
    unsigned short limit;         // Limit of the GDT, i.e., its size minus one.
    unsigned int base;            // Linear base address of the GDT.
} __attribute__((packed));

// -----------------------
// GDT Functions
// -----------------------
// Declarations of functions related to GDT setup.

// Initializes the GDT with predefined segments.
void init_gdt();

// Creates and returns a GDT entry configured with specified parameters.
struct gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

// External declaration of the assembly function to load the GDTR register.
extern void load_gdt(void);

#endif // GDT_H
