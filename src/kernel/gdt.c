#include <yros/pmm.h>

static struct {
    u64_t zero;
    u64_t kernel_code;
    u64_t kernel_data;
    u64_t user_code;
    u64_t user_data;
} gdt = {
    0x0000000000000000,
	0x00209a0000000000,
	0x0040920000000000,
	0x0020fa0000000000,
	0x0040f20000000000,
};

static struct {
    const u16_t size;
    u64_t base;
} __packed__ gdt_ptr = {
    sizeof(gdt) - 1,
    (u64_t)&gdt,
};

//static u32_t gdt_ptr_addr = (u64_t)&gdt_ptr; //& 0xffffffff;
