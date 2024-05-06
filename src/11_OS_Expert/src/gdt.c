// -----------------------------------------------------------------------------
// File: 11_OS_Expert/src/gdt.c
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description:
// Contains the implementation of the functions for setting up and
// managing the Global Descriptor Table (GDT) for the operating system. The GDT
// is essential for defining different memory segments which the CPU uses to
// enforce access control and provide different execution contexts.
// -----------------------------------------------------------------------------

#include "gdt.h"

// Define an array of GDT entries. The number of entries can be adjusted as needed.
struct gdt_entry gdt_entries[5];
struct gdt_ptr gdt_ptr;

// Function:
// create_gdt_entry
// 
// Description: 
// Initializes and returns a GDT entry.
//
//   @param uint32_t base - The base address of the segment.
//   @param uint32_t limit - The limit (size) of the segment.
//   @param uint8_t access - Access flags determining the segment type, privilege levels, etc.
//   @param uint8_t granularity - Flags to control granularity, size, and scope of the segment.
//
//   @returns struct gdt_entry - The configured GDT entry.
struct gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) 
{
    struct gdt_entry entry;

    // Set up the segment base address and limit.
    entry.base_low = base & 0xFFFF;
    entry.base_middle = (base >> 16) & 0xFF;
    entry.base_high = (base >> 24) & 0xFF;
    entry.limit_low = limit & 0xFFFF;

    // Configure granularity and 4K block size if necessary.
    entry.granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

    // Set access rights (read/write, executable, etc.)
    entry.access = access;

    return entry;
}

// Function: 
// init_gdt
// 
// Description: 
// Sets up the GDT with necessary segments and loads it into the CPU.
void init_gdt() {
    // Initialize GDT entries:
    // Entry 0: Null segment
    gdt_entries[0] = create_gdt_entry(0, 0, 0, 0);  // A required null descriptor.

    // Entry 1: Kernel mode code segment (Ring 0, executable, readable)
    gdt_entries[1] = create_gdt_entry(0, 0xFFFFFFFF, GDT_CODE_EXEC_READ, GDT_FLAG_RING0 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // Entry 2: Kernel mode data segment (Ring 0, readable/writable)
    gdt_entries[2] = create_gdt_entry(0, 0xFFFFFFFF, GDT_DATA_READ_WRITE, GDT_FLAG_RING0 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // Entry 3: User mode code segment (Ring 3, executable, readable)
    gdt_entries[3] = create_gdt_entry(0, 0xFFFFFFFF, GDT_CODE_EXEC_READ, GDT_FLAG_RING3 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // Entry 4: User mode data segment (Ring 3, readable/writable)
    gdt_entries[4] = create_gdt_entry(0, 0xFFFFFFFF, GDT_DATA_READ_WRITE, GDT_FLAG_RING3 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);
    
    // Set the GDT pointer (GDTR) which includes the size of the GDT and the address of the array of GDT entries.
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Load the GDT into the GDTR register of the CPU. The actual 'lgdt' instruction is performed in assembly.
    load_gdt();
}
