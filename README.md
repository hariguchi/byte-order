# byte-order
Unsigned integer types storing values in the network byte order.

``` c++
n32: 32-bit unsigned integer in the network byte order.
n16: 16-bit unsigned integer in the network byte order.
```


## Example

``` c++
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef struct __attribute__((packed)) ipv4hdr {
    u8  vhl; /* version + header length */
    u8  tos;
    n16 len;
    n16 id;
    n16 frag; /* flags + fragment offset */
    u8  ttl;
    u8  proto;
    n16 csum;
    n32 sa;
    n32 da;
} ipv4hdr;
```

The following assignment will cause a compilation error.

``` c++
ipv4hdr hdr;
hdr.sa = 0xc0a80101;        /* 192.168.1.1 in the host byte order */
```

The above example shows that using `n32` and `n16` can prevent from
assigning a variable or a constant in the host byte order to
a variable in the network byte order by mistake. Instead of writing as
follows:

``` c++
hdr.sa = htonl(0xc0a80101); /* 192.168.1.1 in the network byte order */
```

`n32` forces to write like as follows:

``` c++
#include <arpa/inet.h>
#include "byte-order.h"

struct ipv4hdr hdr;
int rc;

/* Set the following addresses in the network byte order
 *   hdr.sa = 192.168.1.1
 *   hdr.da = 192.168.1.2
 */
rc = inet_pton(AF_INET, "192.168.1.1", &hdr.sa);
h2n32(&hdr.da, 0xc0a80102);
```
