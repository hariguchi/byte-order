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

The following assignment will cause a compilation error because
the type of `hdr.sa` is not `uint32_t`.

``` c++
ipv4hdr hdr;
hdr.sa = 0xc0a80101;        /* 192.168.1.1 in the host byte order */
```

The above example shows that you can prevent assigning host
byte order values to network byte order variables by using
`n32` and `n16`. If the type of `hdr.sa` were `uint32_t`, the
following statement is used to assign a host byte order value,
for example.

``` c++
hdr.sa = htonl(0xc0a80101); /* 192.168.1.1 in the network byte order */
```

The above statement also causes a compilation error if the type
of `hdr.sa` is `n32`. The function `h2n32()` must be used for
assigning a host byte order value to `n32`. Here is an
example.

``` c++
#include "byte-order.h"

struct ipv4hdr hdr;

h2n32(&hdr.sa, 0xc0a80102); /* hdr.sa = 192.168.1.2 in the network byte order */
```

`n32` can be used with `inet_pton()` like as follows.

``` c++
#include <arpa/inet.h>
#include "byte-order.h"

struct ipv4hdr hdr;
int rc;

/* Set the following addresses in the network byte order
 *   hdr.sa = 192.168.1.2
 *   hdr.da = 192.168.1.3
 */
rc = inet_pton(AF_INET, "192.168.1.2", &hdr.sa);
h2n32(&hdr.da, 0xc0a80103);
```
