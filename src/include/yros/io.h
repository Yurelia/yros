#ifndef YROS_IO_H
#define YROS_IO_H

#include <yros/types.h>

u08_t inb(u16_t port);
void outb(u16_t port, u08_t data);

#endif