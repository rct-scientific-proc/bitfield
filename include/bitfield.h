#ifndef BITFIELD_H
#define BITFIELD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


typedef uint8_t  bit8_t;    /*  8-bits */
typedef uint16_t bit16_t;   /* 16-bits */
typedef uint32_t bit32_t;   /* 32-bits */
typedef uint64_t bit64_t;   /* 64-bits */


/************************************************
 * MACRO:
 *
 * BIT(i): Expands to an integer of the form
 *  2^i, where i is bounded from [0,63]
 ***********************************************/
#define BIT(i) ((bit64_t)1 << ((bit64_t)(i) % 64))


/************************************************
 * MACRO:
 *
 * BITS(var, rval_cptr): Expands to a do-while loop
 *  that takes the rvalue character string and
 *  assigns the variable var to that value as an
 *  integer. Input should contain only '0' and '1'.
 ***********************************************/
#define BITS(bf, rval_cptr) do {\
    (bf) = 0;\
    const char* _bits_ptr = (rval_cptr);\
    while (*_bits_ptr) {\
        (bf) <<= 1;\
        (bf) |= (*_bits_ptr == '1');\
        _bits_ptr++;\
    }\
} while(0)


/************************************************
 * MACRO:
 *
 * SETBIT(bf, i): Expands to an expression that
 *  logical ORs a 1 into the specified bit
 *  position.
 ***********************************************/
#define SETBIT(bf, i)  ((bf) |= (BIT(i)))


/************************************************
 * MACRO:
 *
 * NULLBIT(bf, i): Expands to an expression that
 *  logical ANDs a 0 into the specified bit
 *  position.
 ***********************************************/
#define NULLBIT(bf, i) ((bf) &= ~(BIT(i)))


/************************************************
 * MACRO:
 *
 * ISSET(bf, i): Expands to a logical expression
 *  that returns true if the bit is set at the
 *  ith position in the bitfield.
 ***********************************************/
#define ISSET(bf, i)   (((bf) & (BIT(i))) != 0)


/************************************************
 * MACRO:
 *
 * ISNULL(bf, i): Expands to a logical expression
 *  that returns true if the bit is not set at the
 *  ith position in the bitfield.
 ***********************************************/
#define ISNULL(bf, i)  (((bf) & (BIT(i))) == 0)


/************************************************
 * MACRO:
 *
 * ONES(bf): Efficiently counts the number of bits
 *  that are 1 in the bitfield using compiler
 *  builtins when available, or fallback algorithm.
 ***********************************************/
#if defined(__GNUC__) || defined(__clang__)
    #define ONES(bf) \
        ((sizeof(bf) == 1) ? __builtin_popcount((uint8_t)(bf)) : \
         (sizeof(bf) == 2) ? __builtin_popcount((uint16_t)(bf)) : \
         (sizeof(bf) == 4) ? __builtin_popcount((uint32_t)(bf)) : \
         (sizeof(bf) == 8) ? __builtin_popcountll((uint64_t)(bf)) : 0)
#else
    /* Fallback: Kernighan's algorithm */
    #define ONES(bf) ({ \
        __typeof__(bf) _v = (bf); \
        int _c = 0; \
        while (_v) { _v &= _v - 1; _c++; } \
        _c; \
    })
#endif


/************************************************
 * MACRO:
 *
 * ZEROS(bf): Efficiently counts the number of bits
 *  that are 0 in the bitfield.
 ***********************************************/
#define ZEROS(bf) ((sizeof(bf) * 8) - ONES(bf))


/************************************************
 * MACRO:
 *
 * TOGGLEBIT(bf, i): Toggles (flips) the bit at
 *  the ith position in the bitfield.
 ***********************************************/
#define TOGGLEBIT(bf, i) ((bf) ^= (BIT(i)))


/************************************************
 * MACRO:
 *
 * CLEARBIT(bf, i): Alias for NULLBIT - clears
 *  the bit at the ith position.
 ***********************************************/
#define CLEARBIT(bf, i) NULLBIT(bf, i)


/************************************************
 * MACRO:
 *
 * GETBIT(bf, i): Returns the value (0 or 1) of
 *  the bit at the ith position.
 ***********************************************/
#define GETBIT(bf, i) (ISSET(bf, i) ? 1 : 0)


/************************************************
 * MACRO:
 *
 * BITMASK(n): Creates a bitmask with the lower
 *  n bits set to 1. Example: BITMASK(3) = 0b111
 ***********************************************/
#define BITMASK(n) (((bit64_t)1 << (n)) - 1)


/************************************************
 * MACRO:
 *
 * GETBITS(bf, start, len): Extracts len bits
 *  starting from position start.
 ***********************************************/
#define GETBITS(bf, start, len) (((bf) >> (start)) & BITMASK(len))


/************************************************
 * MACRO:
 *
 * SETBITS(bf, start, len, val): Sets len bits
 *  starting from position start to the value val.
 ***********************************************/
#define SETBITS(bf, start, len, val) do { \
    (bf) = ((bf) & ~(BITMASK(len) << (start))) | (((val) & BITMASK(len)) << (start)); \
} while(0)


/************************************************
 * MACRO:
 *
 * CLEARBITS(bf, start, len): Clears len bits
 *  starting from position start.
 ***********************************************/
#define CLEARBITS(bf, start, len) ((bf) &= ~(BITMASK(len) << (start)))


#ifdef __cplusplus
}
#endif

#endif

