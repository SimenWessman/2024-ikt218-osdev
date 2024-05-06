#include "timer.h"
#include "pic.h"  // You need methods to send commands to the PIC
#include "idt.h"
#include <libc/stdio.h>

#define PIT_FREQ 1193180
#define TIMER_IRQ 0x20  // IRQ0, after PIC remap
#define SECOND 18 // Ticks in a second.

void init_timer(uint32_t frequency) {
    // The divisor must be small enough to fit into 16 bits.
    uint32_t divisor = PIT_FREQ / frequency;
    
    // Send command byte to PIT control port
    outb(0x43, 0x36);  // Command for repeating mode

    // Divisor has to be sent byte-wise, so split it into upper/lower bytes.
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF );

    // Send frequency divisor to PIT data port
    outb(0x40, low);
    outb(0x40, high);

    // Register the timer handler in your IDT setup or another appropriate location
    idt_set_gate(TIMER_IRQ, (uint32_t)timer_handler, 0x08, 0x8E);
}

void timer_handler(void) 
{
    static uint32_t tick = 0;
    tick++;
    // if(tick % 18 == 0) 
    // {
    //     printf("One second has passed!\n");
    // }
    // Acknowledge the interrupt to PIC
    outb(0x20, 0x20);  // Send EOI to the master PIC
    __asm__("sti");  // Re-enable interrupts.
}

