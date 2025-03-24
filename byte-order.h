#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__

#include <stdio.h>
#include <arpa/inet.h>

/*

byte-order.h: unsigned integer types in the network byte order

MIT License

Copyright (c) 2025 Yoichi Hariguchi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/**
 * @name  n32
 * @brief 32-bit unsigned integer in the network byte order (NBO).
 */
struct _n32 {
    union {
        uint32_t val; /* in the network byte order */
        uint8_t a[4];
    };
};
typedef struct _n32 n32;
typedef struct _n32 nint32_t;

/**
 * @name  h2n32
 *
 * @brief Set 'obj->val' to 'val' in the NBO.
 *        'val' must be in the host byte order (HBO).
 *
 * @param[in] obj Pointer to 'n32'.
 * @param[in] val An 32-bit integer in the HBO.
 */
static inline void
h2n32(n32* obj, uint32_t val)
{
    obj->val = htonl(val);
}

/**
 * @name  n2h32
 *
 * @brief Convert 'obj->val' into the HBO and return it.
 *
 * @param[in] obj Pointer to 'n32'.
 *
 * @retval uint32_t 'obj->val' in the HBO.
 */
static inline uint32_t
n2h32(n32* obj)
{
    return ntohl(obj->val);
}

/**
 * @name  naddh32
 *
 * @brief 'obj->val += val': 'ovj->val' is in the NBO, 'val' is in the HBO.
 *
 * @param[in] obj Pointer to 'n32'.
 * @param[in] val A 32-bit integer in the HBO.
 */
static inline void
naddh32(n32* obj, uint32_t val)
{
    uint32_t tmp;

    tmp = n2h32(obj);
    tmp += val;
    h2n32(obj, tmp);
}

/**
 * @name  hsubn32
 *
 * @brief Return 'val - obj->val': 'ovj->val' (NBO), 'val' (HBO)
 *
 * @param[in] obj Pointer to 'n32'.
 *
 * @retval uint32_t 'obj->val' in the HBO.
 */
static inline uint32_t
hsubn32(n32* obj, uint32_t val)
{
    uint32_t tmp;

    tmp = n2h32(obj);

    return (val - tmp);
}

/**
 * @name  nsubh32
 *
 * @brief 'obj->val' is in the NBO, 'val' is in the HBO.
 *   1. 'obj->val -= val' (if 'dst' is NULL)
 *   2. 'dst = obj->val - val'
 *
 * @param[in] obj Pointer to 'n32'.
 * @param[in] dst NULL or a pointer to 'n32'. if 'dst' is NULL,
 *                'obj->val' is updated. Otherwise 'dst->val' is updated.
 * @param[in] val A 32-bit integer in the HBO.
 */
static inline void
nsubh32(n32* obj, n32* dst, uint32_t val)
{
    uint32_t tmp;

    tmp = n2h32(obj);
    tmp -= val;
    if (dst == NULL) {
        dst = obj;
    }
    h2n32(dst, tmp);
}

/**
 * @name  n32dump
 *
 * @brief Return each byte of 'obj->val' in memory as a string.
 *
 * @param[in] obj Pointer to 'n32'.
 * @param[in] dst Pointer to a string buffer where the result is stored.
 * @param[in] val A 32-bit integer in the HBO.

 * @retval char* The value of 'dst'.
 */
static inline char*
n32dump(n32* obj, char* dst, int n)
{
    snprintf(dst, n, "%2x %2x %2x %2x",
             obj->a[0], obj->a[1], obj->a[2], obj->a[3]);

    return dst;
}

/**
 * @name  n16
 * @brief 16-bit unsigned integer in the network byte order (NBO).
 */
struct _n16 {
    union {
        uint16_t val; /* in the network byte order */
        uint8_t a[2];
    };
};
typedef struct _n16 n16;
typedef struct _n16 nint16_t;

/**
 * @name  h2n16
 *
 * @brief Set 'obj->val' to 'val' in the NBO.
 *        'val' must be in the host byte order (HBO).
 *
 * @param[in] obj Pointer to 'n16'.
 * @param[in] val An 16-bit integer in the HBO.
 */
static inline void
h2n16(n16* obj, uint16_t val)
{
    obj->val = htons(val);
}

/**
 * @name  n2h16
 *
 * @brief Convert 'obj->val' into the HBO and return it.
 *
 * @param[in] obj Pointer to 'n16'.
 *
 * @retval uint16_t 'obj->val' in the HBO.
 */
static inline uint16_t
n2h16(n16* obj)
{
    return ntohs(obj->val);
}

/**
 * @name  naddh16
 *
 * @brief 'obj->val += val': 'ovj->val' is in the NBO, 'val' is in the HBO.
 *
 * @param[in] obj Pointer to 'n16'.
 * @param[in] val A 16-bit integer in the HBO.
 */
static inline void
naddh16(n16* obj, uint16_t val)
{
    uint16_t tmp;

    tmp = n2h16(obj);
    tmp += val;
    h2n16(obj, tmp);
}

/**
 * @name  hsubn16
 *
 * @brief Return 'val - obj->val': 'ovj->val' (NBO), 'val' (HBO)
 *
 * @param[in] obj Pointer to 'n16'.
 *
 * @retval uint16_t 'obj->val' in the HBO.
 */
static inline uint16_t
hsubn16(n16* obj, uint16_t val)
{
    uint16_t tmp;

    tmp = n2h16(obj);

    return  val - tmp;
}

/**
 * @name  nsubh16
 *
 * @brief 'obj->val' is in the NBO, 'val' is in the HBO.
 *   1. 'obj->val -= val' (if 'dst' is NULL)
 *   2. 'dst = obj->val - val'
 *
 * @param[in] obj Pointer to 'n16'.
 * @param[in] dst NULL or a pointer to 'n16'. if 'dst' is NULL,
 *                'obj->val' is updated. Otherwise 'dst->val' is updated.
 * @param[in] val A 16-bit integer in the HBO.
 */
static inline void
nsubh16(n16* obj, n16* dst, uint16_t val)
{
    uint16_t tmp;

    tmp = n2h16(obj);
    tmp -= val;
    if (dst == NULL) {
        dst = obj;
    }
    h2n16(obj, tmp);
}

/**
 * @name  n16dump
 *
 * @brief Return each byte of 'obj->val' in memory as a string.
 *
 * @param[in] obj Pointer to 'n16'.
 * @param[in] dst Pointer to a string buffer where the result is stored.
 * @param[in] val A 16-bit integer in the HBO.

 * @retval char* The value of 'dst'.
 */
static inline char*
n16dump(n16* obj, char* dst, int n)
{
    snprintf(dst, n, "%2x %2x", obj->a[0], obj->a[1]);

    return dst;
}


#endif /* __BYTE_ORDER_H__ */
