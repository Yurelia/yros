#ifndef YROS_IDT_H
#define YROS_IDT_H

#include <yros/types.h>

typedef struct {
    u16_t isr0;
    u16_t seg;
    u08_t ist;
    u08_t attr;
    u16_t isr1;
    u32_t isr2;
    u32_t res;
} __packed__ idt_gate_t;

void init_interrupt();

#endif
