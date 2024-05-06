#ifndef MOUSE_H
#define MOUSE_H
#include <libc/stdint.h>
#include <libc/stdbool.h>

void handle_text_selection(int x, int y);

void init_mouse(void);
void mouse_handler(void);

#endif // MOUSE_H
