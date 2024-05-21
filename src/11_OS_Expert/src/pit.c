#include "kernel/pit.h"
#include "kernel/memory.h"
#include <libc/stdio.h>

void init_pit(void) 
{
    outb(PIT_CMD_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(DIVIDER & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((DIVIDER >> 8) & 0xFF));

    printf("PIT initialized\n");
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start = get_tick_count();
    uint32_t ticks = milliseconds * TICKS_PER_MS;
    uint32_t final = start + ticks;

    if (final < start) 
    {
        while (get_tick_count() >= start) 
        {
            // Do nothing
        }
    }

    while (get_tick_count() < final) 
    {
        // Do nothing
        //printf("Tick: %d\n", get_tick_count());
        asm ("nop");
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start = get_tick_count();
    uint32_t ticks = milliseconds * TICKS_PER_MS;
    while (get_tick_count() < start + ticks) {
        __asm__ __volatile__(
            "sti\n\t"
            "hlt\n\t");
    }
}
