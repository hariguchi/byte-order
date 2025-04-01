#include "byte-order.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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


char*
memdump(uint8_t* start, size_t nbytes, char* dst, size_t bufsize)
{
    int i;
    int n;
    uint8_t* p = start;
    char* s = dst;

    for (;;) {
        n = snprintf(s, bufsize - (s - dst), "%p: ", p);
        s += n;
        if ((s - dst) >= bufsize) {
            dst[bufsize-1] = '\0';
            return dst;
        }
        for (i = 0; i < 16; ++i) {
            if ((p - start) >= nbytes) {
                *s = '\0';
                return dst;
            }
            n = snprintf(s, bufsize - (s - dst), "%02x ", *p++);
            s += n;
            if ((s - dst) >= bufsize) {
                dst[bufsize-1] = '\0';
                return dst;
            }
        }
        *(s - 1) = '\n';
    }
}


void
ipv4hdrTest()
{
    ipv4hdr hdr;
    int rc;
    char buf[128];

    memset(&hdr, 0, sizeof(hdr));
    hdr.vhl = 0x45;
    h2n16(&hdr.id, 0x1234);
    h2n32(&hdr.da, 0xc0a80102);
    rc = inet_pton(AF_INET, "192.168.1.3", &hdr.sa);
    if (rc == 1) {
        n32dump(&hdr.sa, buf, sizeof(buf));
        printf("hdr.sa: %s\n", buf);
        assert(hdr.sa.a[0] == 0xc0);
        assert(hdr.sa.a[1] == 0xa8);
        assert(hdr.sa.a[2] == 0x01);
        assert(hdr.sa.a[3] == 0x03);

        n16dump(&hdr.id, buf, sizeof(buf));
        printf("hdr.id: %s\n", buf);
        assert(hdr.id.a[0] == 0x12);
        assert(hdr.id.a[1] == 0x34);
        memdump((uint8_t*)&hdr , sizeof(hdr), buf, sizeof(buf));
        printf("%s\n", buf);
    } else {
        if (rc < 0) {
            fprintf(stderr, "ERROR: inet_pton(): %d\n", errno);
        } else if (rc == 0) {
            fprintf(stderr, "ERROR: wrong string.\n");
        }
        exit(1);
    }
}


int
main (int argc, char* argv[])
{
    n32 addrNet1;
    n32 addrNet2;
    u32 addrHost;
    n16 portNet1;
    n16 portNet2;
    u16 portHost;
    char buf[16];
    char *s;

    /* *** 32 bits *** */

    /* Assignment test
     * 'addrNet1' is in the network byte order
     * 'addrNet2' is a copy of of 'addrNet1' (assignment works)
     * 'addrHost' is in the host byte order
     */
    h2n32(&addrNet1, 0x12345678);
    addrNet2 = addrNet1;
    addrHost = n2h32(&addrNet1);

    assert(addrNet1.val == 0x78563412);
    assert(addrNet2.val == 0x78563412);
    assert(addrHost == 0x12345678);

    n32dump(&addrNet1, buf, sizeof(buf));
    printf("addrNet1: %s\n", buf);
    n32dump(&addrNet2, buf, sizeof(buf));
    printf("addrNet2: %s\n", buf);
    s = (char*)(&addrHost);
    printf("addrHost: %2x %2x %2x %2x\n", s[0], s[1], s[2], s[3]);
    printf("addrHost: 0x%08x\n", addrHost);

    /* Cast test.
     */
    addrNet2 = *((n32*)(&addrHost));

    assert(addrNet2.val == 0x12345678);

    n32dump(&addrNet2, buf, sizeof(buf));
    printf("addrNet2: %s\n", buf);

    /* Arithmetic test
     */
    addrNet2 = addrNet1;
    naddh32(&addrNet2, 5);
    addrHost = n2h32(&addrNet2);

    assert(addrHost == 0x1234567d);
    printf("nadd32(): addrHost: 0x%08x (+5)\n", addrHost);

    nsubh32(&addrNet2, NULL, 5);
    addrHost = n2h32(&addrNet2);

    assert(addrHost == 0x12345678);
    printf("nsubh32(NULL): addrHost: 0x%08x (5-5)\n", addrHost);
    
    nsubh32(&addrNet2, &addrNet2, 5);
    addrHost = n2h32(&addrNet2);

    assert(addrHost == 0x12345673);
    printf("nsubh32(): addrHost: 0x%08x (-5)\n", addrHost);

    addrHost = hsubn32(&addrNet1, 0x8765ba98);

    assert(addrHost == 0x75316420);
    printf("hsubn32(): addrHost: 0x%08x (-0x8765ba98)\n", addrHost);


    /* *** 16 bits *** */

    /* Assignment test
     * 'portNet1' is in the network byte order
     * 'portNet2' is a copy of of 'portNet1' (assignment works)
     * 'portHost' is in the host byte order
     */
    h2n16(&portNet1, 0x1234);
    portNet2 = portNet1;
    portHost = n2h16(&portNet1);

    assert(portNet1.val == 0x3412);
    assert(portNet2.val == 0x3412);
    assert(portHost == 0x1234);

    n16dump(&portNet1, buf, sizeof(buf));
    printf("portNet1: %s\n", buf);
    n16dump(&portNet2, buf, sizeof(buf));
    printf("portNet2: %s\n", buf);
    s = (char*)(&portHost);
    printf("portHost: %2x %2x\n", s[0], s[1]);
    printf("portHost: 0x%04x\n", portHost);

    /* Cast test.
     */
    portNet2 = *((n16*)(&portHost));

    assert(portNet2.val == 0x1234);

    n16dump(&portNet2, buf, sizeof(buf));
    printf("portNet2: %s\n", buf);

    /* Arithmetic test
     */
    portNet2 = portNet1;
    naddh16(&portNet2, 5);
    portHost = n2h16(&portNet2);

    assert(portHost == 0x1239);
    printf("nadd16(): portHost: 0x%04x (+5)\n", portHost);

    nsubh16(&portNet2, NULL, 5);
    portHost = n2h16(&portNet2);

    assert(portHost == 0x1234);
    printf("nsubh16(NULL): portHost: 0x%04x (5-5)\n", portHost);
    
    nsubh16(&portNet2, &portNet2, 5);
    portHost = n2h16(&portNet2);

    assert(portHost == 0x122f);
    printf("nsubh16(): portHost: 0x%04x (-5)\n", portHost);

    portHost = hsubn16(&portNet1, 0x8765);

    assert(portHost == 0x7531);
    printf("hsubn16(): portHost: 0x%04x (-0x8765)\n", portHost);

    ipv4hdrTest();

    exit(0);
    return 0;
}
