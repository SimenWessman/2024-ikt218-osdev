#ifndef TIMER_H
#define TIMER_H
#include <libc/stdint.h>

void init_timer(uint32_t frequency);
void timer_handler(void);

#endif // TIMER_H
