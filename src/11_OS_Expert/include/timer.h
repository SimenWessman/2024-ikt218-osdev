#ifndef TIMER_H
#define TIMER_H
#include <libc/stdint.h>

void init_timer(uint32_t frequency);
void timer_handler(void);
uint32_t get_tick_count(void);

#endif // TIMER_H
