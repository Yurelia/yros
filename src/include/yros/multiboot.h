#ifndef YROS_MULTIBOOT_H
#define YROS_MULTIBOOT_H

#include <yros/types.h>
#include <yros/framebuffer.h>

#define MULTIBOOT2_MAGIC 0x36d76289

#define MBOOT_TAG_END 0
#define MBOOT_TAG_MMAP 6
#define MBOOT_TAG_FRAMEBUFFER       8
#define MBOOT_TAG_ACPIV2 15

#define MULTIBOOT_MEMORY_AVAILABLE        1
#define MULTIBOOT_MEMORY_RESERVED         2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS              4
#define MULTIBOOT_MEMORY_BADRAM           5

// multiboot tag
typedef struct {
    u32_t type;
    u32_t size;
} __packed__ mboot_header_t;

// multiboot mmap entry
typedef struct {
    u64_t addr;
    u64_t len;
    u32_t type;
    u32_t res;
} __packed__ mboot_mmap_entry_t;

// multiboot mmap tag
typedef struct {
    mboot_header_t header;
    u32_t entry_size;
    u32_t entry_version;
    mboot_mmap_entry_t entries[];
} __packed__ mboot_mmap_t;

// multiboot framebuffer tag
typedef struct {
    mboot_header_t header;
    u64_t addr;
    u32_t pitch;
    u32_t width;
    u32_t height;
    u08_t bpp;
    u08_t type;
    u16_t res;
    u08_t r_pos;
    u08_t r_size;
    u08_t g_pos;
    u08_t g_size;
    u08_t b_pos;
    u08_t b_size;
} __packed__ mboot_framebuffer_t;

// multiboot acpi v2 tag
typedef struct {
    mboot_header_t header;
    
    char  Signature[8];
    u08_t Checksum;
    char  OEMID[6];
    u08_t Revision;
    u32_t RsdtAddress;

    u32_t Length;
    u64_t XsdtAddress;
    u08_t ExtendedChecksum;
    u08_t Reserved[3];
} __packed__ mboot_acpiv2_t;

extern mboot_header_t *mboot_tags[22];

void init_boot_tags();

#endif