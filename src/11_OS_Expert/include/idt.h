#ifndef IDT_H
#define IDT_H

// 
#include "libc/string.h"
#include "libc/stdint.h"

extern void keyboard_handler(void);

struct idt_entry
{
    uint16_t base_lo;  // Lower 16 bits of handler function address
    uint16_t sel;      // Kernel segment selector
    uint8_t  always0;  // This must always be zero
    uint8_t  flags;    // Flags
    uint16_t base_hi;  // Upper 16 bits of handler function address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr 
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Function prototypes
void init_idt();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif