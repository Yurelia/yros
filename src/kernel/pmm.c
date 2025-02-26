#include <yros/pmm.h>
#include <yros/multiboot.h>

extern u64_t kernel_end[], kaddr[];

static struct {
	u64_t *map;
	u64_t size;
	u64_t stats[10];
	pmm_page_t zone[MAX_ORDER];
} pmm;

static __inline__ void pmm_list_add(pmm_page_t *page, pmm_page_t *list)
{
	pmm.stats[LOG2(list->size >> 12)]++;
	*page = (pmm_page_t){list->size, list->prev, list};
	list->prev = list->prev->next = page;
}

static __inline__ u64_t pmm_list_del(pmm_page_t *page)
{
	pmm.stats[LOG2(page->size >> 12)]--;
	page->prev->next = page->next;
	page->next->prev = page->prev;
	return (u64_t)page;
}

static __inline__ void pmm_merge(u64_t page, u64_t order)
{
	u64_t *size = (u64_t*)&pmm.zone[order];

	for (u64_t buddy;
		*(u64_t*)(buddy = page ^ *size) == *size
		&& *size < 1 << (MAX_ORDER + 11)
		&& !(pmm.map[buddy >> 18] & 1ll << (buddy >> 12 & 63));
		page &= ~*size, size += sizeof(pmm_page_t) / 8)
		pmm_list_del((pmm_page_t*)buddy);
	
	pmm_list_add((pmm_page_t*)page, (pmm_page_t*)size);
}

static __inline__ u64_t pmm_split(u64_t order)
{
	pmm_page_t *need = &pmm.zone[order];
	if (need->next != need) return pmm_list_del(need->next);

	pmm_page_t *seek = need + 1;
	for (; seek < &pmm.zone[MAX_ORDER] && seek->next == seek; seek++);
	if (seek == &pmm.zone[MAX_ORDER]) return 0;

	u64_t page = pmm_list_del(seek->next), pend = page + seek->size;
	for (; page < pend - need->size; page += need->size)
		pmm_list_add((pmm_page_t*)page, need);
	return page;
}

void pmm_free_page(u64_t page)
{
	pmm.map[page >> 18] &= ~(1ll << (page >> 12 & 63));
	pmm_merge(page, 0);
}

void pmm_free_pages(u64_t page, u64_t order)
{
	pmm.map[page >> 18] &= ~(~(~0ll << (1 << order)) << (page >> 12 & 63));
	for (size_t i = 0; i < 1 << order >> 6; i++)
		pmm.map[(page >> 18) + i] = 0ll;
	pmm_merge(page, order);
}

u64_t pmm_alloc_page()
{
	u64_t page = pmm_split(0);
	pmm.map[page >> 18] |= 1ll << (page >> 12 & 63);
	return page;
}

u64_t pmm_alloc_pages(u64_t order)
{
	u64_t page = pmm_split(order);
	pmm.map[page >> 18] |= ~(~0ll << (1 << order)) << (page >> 12 & 63);
	for (size_t i = 0; i < 1 << order >> 6; i++) pmm.map[(page >> 18) + i] = ~0ll;
	return page;
}

void wptable(u64_t level, u64_t phy, u64_t vir, u64_t attr)
{
	u64_t addr = 0xffffff7fbfdfe000 << 9 * (4 - level);
	addr |= (vir >> (9 * level + 3) << 3) | 0xffff000000000000;
	*(u64_t*)addr = phy | attr;
}

void init_memory()
{
	mboot_header_t *memtag = mboot_tags[MBOOT_TAG_MMAP];
	mboot_mmap_entry_t *entry = (void*)memtag + memtag->size;
	for (--entry; entry->type != MULTIBOOT_MEMORY_AVAILABLE; entry--);
	u64_t map_size = (entry->addr + entry->len) >> 15;
	pmm.map = (u64_t*)((u64_t)kernel_end & 0x00000000fffffff8) + 1;

	for (size_t i = 0; i < map_size >> 3; i++)
		pmm.map[i] = ~0;
	for (size_t i = 0; i < MAX_ORDER; i++)
		pmm.zone[i] = (pmm_page_t){0x1000 << i, &pmm.zone[i], &pmm.zone[i]};
	for (size_t i = 4; i < map_size >> (30 - 15); i++)
		wptable(3, i * 0x40000000, i * 0x40000000, 0b10000011);
	for (; entry->addr != 0; entry--) {
 		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
			u64_t beg = entry->addr & (~0 << 12);
			u64_t end = (entry->addr + entry->len) & (~0 << 12);
			if (entry->addr == 0x100000)
				beg = ((u64_t)pmm.map + map_size + 4095) & (~0 << 12);

			u64_t size = 0, max_size = 1 << (MAX_ORDER + 11);
			for (; (size = beg & -beg) < max_size && beg < end - size; beg += size)
				pmm_free_pages(beg, LOG2(size >> 12));
			for (; beg <= end - max_size; beg += max_size)
				pmm_free_pages(beg, MAX_ORDER - 1);
			for (; (size = end & -end) && beg <= (end -= size);)
				pmm_free_pages(end, LOG2(size >> 12));

		}
	}

	/*u08_t byte = ~0, *ptr = (u08_t*)pmm.map;
    for (size_t addr, i = 0; i < map_size; i++, ptr++) {
		if (*ptr != byte) {
			addr = i << 15;
			byte = *ptr;
			int tmpppp = 1;
		}
	}*/
}