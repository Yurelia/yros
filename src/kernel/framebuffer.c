#include <yros/framebuffer.h>
#include <yros/multiboot.h>

extern u64_t psfu_beg, psfu_end, psfu_size;

static struct fb {
    u08_t *ptr;
    u08_t *fbbase;
    u08_t *chbase;

    u32_t pw;
    u32_t ph;
    u32_t xbytes;
    u32_t ybytes;

    u32_t gw;
    u32_t gh;
    u32_t gsize;
    u32_t cw;
    u32_t ch;
    u32_t nline;
} fb;

inline void fb_setpos(u32_t px, u32_t py)
{
    fb.ptr = fb.fbbase + px * fb.xbytes + py * fb.ybytes;
}

inline void fb_putpxl(u32_t clr)
{
    *fb.ptr++ = (u08_t)(clr >>= 0);
    *fb.ptr++ = (u08_t)(clr >>= 8);
    *fb.ptr++ = (u08_t)(clr >>= 8);
}

void fb_putstr(const char *str, u32_t px, u32_t py, u32_t fg, u32_t bg)
{
    for (u32_t c = 0; str[c] != 0; c++) {
        u08_t *glyph = fb.chbase + fb.gsize * str[c];
        for (u32_t y = 0; y < fb.gh; y++) {
            fb_setpos(px + c * fb.gw, py + y);
            for (u32_t x = 0; x < fb.gw; x++)
                fb_putpxl(glyph[x >> 3] & 0x80 >> (x & 0b111) ? fg : bg); 
            glyph += (fb.gw + 7) / 8;
        }
    }
}

void fb_logk(logk_t type, const char *info)
{
    const char *str[6] = {"[ DEBUG ]", "[ ERROR ]", "[MESSAGE]",
                          "[SUCCESS]", "[VERBOSE]", "[WARNING]"};
	const u32_t clr[6] = { 0x0000ff  ,  0xff0000  ,  0xffffff  ,
                           0x00ff00  ,  0x888888  ,  0xffff00  };
    
    if (fb.nline == fb.ch) {
        fb.nline--;
        u64_t *next = (u64_t*)fb.fbbase,
              *prev = (u64_t*)((u64_t)next + fb.gh * fb.ybytes),
              *pend = (u64_t*)(fb.fbbase + (fb.ch - 1) * fb.gh * fb.ybytes),
              *nend = (u64_t*)((u64_t)pend + fb.gh * fb.ybytes);
        while (next < pend) *next++ = *prev++;
        for (next = pend; next < nend; next++) *next = 0;
    }
	fb_putstr(str[type], 0, fb.nline * fb.gh, clr[type], 0x000000);
	fb_putstr(info, 10 * fb.gw, fb.nline++ * fb.gh, 0xffffff, 0x000000);
}

void init_screen()
{
    mboot_framebuffer_t *fbtag = (mboot_framebuffer_t*)mboot_tags[MBOOT_TAG_FRAMEBUFFER];
    psf_header_t *psfinfo = (psf_header_t*)&psfu_beg;
    fb = (struct fb){0, (u08_t*)fbtag->addr, (u08_t*)psfinfo + psfinfo->size,
        fbtag->width, fbtag->height, fbtag->bpp / 8, fbtag->pitch,
        psfinfo->width, psfinfo->height, psfinfo->glyph_size,
        fbtag->width / psfinfo->width, fbtag->height / psfinfo->height, 0};
    
    //for (size_t i = 0; i < 27; i++) fb_logk(DEBUG, "test!!!!!!!!!!!!!!!!!!!!!!!");
    fb_logk(SUCCESS, "init screen");
}