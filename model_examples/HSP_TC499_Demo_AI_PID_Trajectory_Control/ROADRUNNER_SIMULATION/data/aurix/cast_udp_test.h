#ifndef CAST_H_
#define CAST_H_
#include <stdint.h>
static inline uint32_t single2u32(float x)
{
    return *(uint32_t*)&x;
}
#endif// CAST_H_
