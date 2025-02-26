#ifndef YROS_FRAMEBUFFER_H
#define YROS_FRAMEBUFFER_H

#include <yros/types.h>

typedef enum {
    DEBUG, ERROR, MESSAGE, SUCCESS, VERBOSE, WARNING
} logk_t;

typedef struct {
    u32_t magic;         /* magic bytes to identify PSF */
    u32_t version;       /* zero */
    u32_t size;          /* offset of bitmaps in file, 32 */
    u32_t flags;         /* 0 if there's no unicode table */
    u32_t glyph_num;     /* number of glyphs */
    u32_t glyph_size;    /* size of each glyph */
    u32_t height;        /* height in pixels */
    u32_t width;         /* width in pixels */
} psf_header_t;

void fb_setpos(u32_t px, u32_t py);

void fb_putpxl(u32_t clr);

void fb_putstr(const char *str, u32_t px, u32_t py, u32_t fg, u32_t bg);

void init_screen();

#endif