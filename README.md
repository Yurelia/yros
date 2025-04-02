# YROS

This is a simple 64 bit kernel I wrote because I am curious about how a kernel works.

It is still in progress...

## Completed work

The following features have already been implemented:

* Multiboot v2
* Long mode
* Kernel placed in higher half
* Basic framebuffer support (and PSF v2 font is also available to be shown on screen)
* Support ACPI v2
* Basic support for local apic and io apic (keyboard IRQ available)
* Enabled paging
* Support loading 4KB, 2MB, 1GB pages (pages that are available by x86 architecture)
* Support accessing page tables via recursion technique
* Implemented buddy allocator (for physical memory allocation)

## Scheduled work

* (Currently working on) lazy buddy allocator (paper: https://cs.au.dk/~gerth/papers/actainformatica05.pdf; not really sure whether to implement it or not)
* Full memory allocation mechanism (virtual memory allocator and kheap and so on)
* System call support
* Multi-thread support
* Thread switching, thread sleeping, spinlocks, and many other useful multithread features
* Basic userspace supprt
