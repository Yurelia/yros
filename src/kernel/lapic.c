#include <yros/lapic.h>
#include <yros/io.h>
#include <yros/multiboot.h>

static u64_t lapic_base;

inline u64_t rdmsr(u32_t address)
{
    u32_t low = 0, high = 0;
    asm ("movl %2, %%ecx\n\t" 
        "rdmsr\n\t"
        : "=a" (low), "=d" (high)
        : "g" (address)
    );

    return (u64_t)low | ((u64_t)high << 32);
}

inline void wrmsr(u32_t address, u64_t value)
{
    asm ("wrmsr"
    :: "a" ((u32_t)value), "d"(value >> 32), "c"(address)
    );
}

static inline u32_t rdapicr(u64_t offset)
{
    return *(u32_t*)(lapic_base + offset);
}

static inline void wrapicr(u64_t offset, u32_t value)
{
    *(u32_t*)(lapic_base + offset) = value;
}

void init_apic()
{
    lapic_base = rdmsr(IA32_APIC_BASE) & 0xfffff000;
    //u32_t tmp = *(u32_t*)(lapic_base + 0x320);

    outb(PIC_COMMAND_M, 0x11); // Start init sequence of the master pic
    outb(PIC_COMMAND_S, 0x11); // Start init sequence of the slave pic
    outb(PIC_DATA_M,    0x20); // Offset of interrupts received by master pic is 0x20
    outb(PIC_DATA_S,    0x28); // Offset of interrupts received by slave pic is 0x28
    outb(PIC_DATA_M,    0x04); // Indicate that this is a master pic 
    outb(PIC_DATA_S,    0x02); // Indicate that this is a slave pic
    outb(PIC_DATA_M,    0x01); // Config master pic to use 8086 mode
    outb(PIC_DATA_S,    0x01); // Config slave pic to use 8086 mode
    //outb(PIC_DATA_M,    0xFF); // Mask all interrupts received by master pic
    //outb(PIC_DATA_S,    0xFF); // Mask all interrupts received by slave pic

    u08_t prev;
    outb(0x70, 0x8A);
    prev = inb(0x71);
    outb(0x70, 0x8A);
    outb(0x71, (prev & 0xF0) | 9);

    outb(0x70, 0x8B);
    prev = inb(0x71);
    outb(0x70, 0x8B);
    outb(0x71, prev | 0x40);

    asm ("sti");
    asm ("hlt");
    //asm ("int $40");
    int a = 1;
    int b = 1;
}

void test_irq()
{
    int b = 1;
    int a = 1;
}