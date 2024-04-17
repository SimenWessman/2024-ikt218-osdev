#include "idt.h"
#include "keyboard.h"
#include "libc/stdio.h"

// Defining the IDT entry structure
struct idt_entry idt_entries[256];

// Defining the IDT pointer structure
struct idt_ptr idtp;

// Set an entry in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) 
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void print_idt_entry(uint8_t num) 
{
    struct idt_entry e = idt_entries[num];
    printf("IDT Entry %d: base_lo=%X, base_hi=%X, sel=%X, flags=%X\n",
        num, e.base_lo, e.base_hi, e.sel, e.flags);
}

// Initialize the IDT
void init_idt() {
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);
    
    // TODO: Add specific gates:
    idt_set_gate(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);

    // Load the IDT
    __asm__ __volatile__("lidt (%0)" : : "r" (&idtp));
    __asm__ __volatile__("sti");

    //print_idt_entry(0x21);
}