#ifndef YROS_PMM_H
#define YROS_PMM_H

#include <yros/types.h>

#define MAX_ORDER 10

typedef struct pmm_page_t {
    u64_t size;
    struct pmm_page_t *prev;
    struct pmm_page_t *next;
} __packed__ pmm_page_t;

u64_t pmm_alloc_page();

u64_t pmm_alloc_pages(u64_t order);

void pmm_free_page(u64_t page);

void pmm_free_pages(u64_t page, u64_t order);

void wptable(u64_t level, u64_t phy, u64_t vir, u64_t attr);

void init_memory();

#endif