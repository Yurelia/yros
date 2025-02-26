#ifndef YROS_LAPIC_H
#define YROS_LAPIC_H

#include <yros/types.h>

#define IA32_APIC_BASE 0x1b

#define PIC_COMMAND_M 0x20
#define PIC_COMMAND_S 0xA0
#define PIC_DATA_M    0x21
#define PIC_DATA_S    0xA1

u64_t rdmsr(u32_t address);

void wrmsr(u32_t address, u64_t value);

void init_apic();

void test_irq();

#endif