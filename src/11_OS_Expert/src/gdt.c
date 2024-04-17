// ------------------
// |     gdt.c      |
// ------------------

#include "gdt.h"

struct gdt_entry gdt_entries[5]; // Adjust the number of entries as needed
struct gdt_ptr gdt_ptr;


struct gdt_entry create_gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    struct gdt_entry entry;

    // Initialize the GDT entry.
    entry.limit_low = limit & 0xFFFF;
    entry.granularity = (limit >> 16) & 0x0F;
    entry.granularity |= granularity & 0xF0;
    entry.access = access;
    entry.base_low = base & 0xFFFF;
    entry.base_middle = (base >> 16) & 0xFF;
    entry.base_high = (base >> 24) & 0xFF;

    return entry;
}

void init_gdt() 
{
    // Initialize GDT entries:
    gdt_entries[0] = create_gdt_entry(0, 0, 0, 0); // Null segment

    // Kernel mode code segment
    gdt_entries[1] = create_gdt_entry(0, 0xFFFFFFFF, GDT_CODE_EXEC_READ, GDT_FLAG_RING0 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // Kernel mode data segment
    gdt_entries[2] = create_gdt_entry(0, 0xFFFFFFFF, GDT_DATA_READ_WRITE, GDT_FLAG_RING0 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // User mode code segment
    gdt_entries[3] = create_gdt_entry(0, 0xFFFFFFFF, GDT_CODE_EXEC_READ, GDT_FLAG_RING3 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);

    // User mode data segment
    gdt_entries[4] = create_gdt_entry(0, 0xFFFFFFFF, GDT_DATA_READ_WRITE, GDT_FLAG_RING3 | GDT_GRANULARITY_4K | GDT_32_BIT_MODE);
    
    // Set up the GDT pointer and limit
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Load the GDT using lgdt...
    load_gdt();
}
