#include <yros/idt.h>

extern u64_t isrt[];
extern void isr_40();

static idt_gate_t idt[256];

static struct {
	u16_t	limit;
	u64_t	base;
} __packed__ idtr;

void set_interrupt(u08_t itrid, u64_t funcaddr)
{
    idt_gate_t *gate = &idt[itrid];
    gate->isr0 = funcaddr & 0xffff;
    gate->seg  = 1 << 3;
    gate->ist  = 0;
    gate->attr = 0x8E;
    gate->isr1 = (funcaddr >> 16) & 0xffff;
    gate->isr2 = (funcaddr >> 32) & 0xffffffff;
    gate->res  = 0;
}

void init_interrupt()
{
	for (size_t i = 0; i < 32; i++) {
        idt_gate_t *gate = &idt[i];
        u64_t isr_addr = isrt[i];

        gate->isr0 = isr_addr & 0xffff;
        gate->seg  = 1 << 3;
        gate->ist  = 0;
        gate->attr = 0x8E;
        gate->isr1 = (isr_addr >> 16) & 0xffff;
        gate->isr2 = (isr_addr >> 32) & 0xffffffff;
        gate->res  = 0;
    }
    set_interrupt(40, (u64_t)&isr_40);

    idtr.base = (u64_t)&idt;
    idtr.limit = (u16_t)sizeof(idt_gate_t) * 256 - 1;

    __asm__ __volatile__("lidt %0": :"g" (idtr));
}
