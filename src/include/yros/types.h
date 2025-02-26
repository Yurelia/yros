#ifndef YROS_TYPES_H
#define YROS_TYPES_H

#define EOS '\0'

#define NULL ((void*)0)

#define __inline__ __attribute__((always_inline)) inline
#define __packed__ __attribute__((packed))

#define LOG2(x) __builtin_ctzll(x)

typedef unsigned long long size_t;

typedef char      i08_t;
typedef short     i16_t;
typedef int       i32_t;
typedef long long i64_t;

typedef unsigned char      u08_t;
typedef unsigned short     u16_t;
typedef unsigned int       u32_t;
typedef unsigned long long u64_t;

#endif
