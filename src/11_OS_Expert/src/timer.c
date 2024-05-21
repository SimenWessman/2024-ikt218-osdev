#include "timer.h"
#include "pic.h"  // You need methods to send commands to the PIC
#include "kernel/pit.h"
#include "idt.h"
#include <libc/stdio.h>

#define PIT_FREQ 1193180
#define TIMER_IRQ 0x20  // IRQ0, after PIC remap
#define SECOND 18 // Ticks in a second.

static uint32_t tick;


void init_timer(uint32_t frequency) 
{
    tick = 0;
    uint32_t divisor = PIT_FREQ / frequency;  // Calculate divisor based on the desired frequency

    outb(0x43, 0x36);  // Command for repeating mode

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, low);
    outb(0x40, high);

    idt_set_gate(TIMER_IRQ, (uint32_t)timer_handler, 0x08, 0x8E);
}

void timer_handler(void) 
{
    
    tick++;
    // if(tick % 18 == 0) 
    // {
    //     printf("One second has passed!\n");
    // }
    // Acknowledge the interrupt to PIC
    //printf("Tick: %d\n", tick);
    outb(0x20, 0x20);  // Send EOI to the master PIC
    __asm__("sti");  // Re-enable interrupts.
}

uint32_t get_tick_count(void)
{
    return tick;
}
