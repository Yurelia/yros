#include <yros/io.h>

inline u08_t inb(u16_t port)
{
    static u08_t data;
    asm volatile ("inb %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

inline void outb(u16_t port, u08_t data)
{
    asm volatile ("outb %%al, %%dx" :: "a" (data), "d" (port));
}