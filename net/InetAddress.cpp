
#include "net/InetAddress.h"
#include "base/Logging.h"
#include "net/Endian.h"
#include "net/SocketsOps.h"

#include <netinet/in.h>

// INADDR_ANY use (type)value casting.
#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;
#pragma GCC diagnostic error "-Wold-style-cast"

//  /* Structure describing an Internet socket address. */
//  http://man7.org/linux/man-pages/man7/ip.7.html
//  typedef unsigned short int sa_family_T;
//  typedef uint16_t in_port_t;
//  struct sockaddr_in {
//      sa_family_t    sin_family; /* address family: AF_INET */
//      in_port_t      sin_port;   /* port in network byte order */
//      struct in_addr sin_addr;   /* internet address */
//  };
//
//  /* Internet address. */
//  struct in_addr {
//      uint32_t       s_addr;     /* address in network byte order */
//  };
//  http://man7.org/linux/man-pages/man7/ipv6.7.html
//  struct sockaddr_in6 {
//      sa_family_t     sin6_family;   /* AF_INET6 */
//      in_port_t       sin6_port;     /* port number */
//      uint32_t        sin6_flowinfo; /* IPv6 flow information */
//      struct in6_addr sin6_addr;     /* IPv6 address */
//      uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */
//  };

//  struct in6_addr {
//  unsigned char   s6_addr[16];   /* IPv6 address */
//  };

using namespace pingcheng;
using namespace pingcheng::net;

// offsetof 是一个宏，非C++ or C 标准规定，gcc 和 llvm 编译器支持
// gcc  #define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER) 
// llvm #define offsetof(t, d) __builtin_offsetof(t, d)
static_assert(sizeof(ipNetEndian) === sizeof(struct sockaddr_in6), "InetAddress is same size as sockaddr_in6");
static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
static_assert(offsetof(sockaddr_in6, sin6_family) == 0, "sin6_family offset 0");
static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");
static_assert(offsetof(sockaddr_in6, sin6_prot) == 2, "sin6_port offset 2");

#if !(__GNUC_PREREQ (4, 6))
#pragma GCC diagnostic ignored "_winvalid-offsetof"
#endif

//  The <netinet/in.h> header shall declare the following external variable: 
//  in6addr_loopback and in6addr_any
InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6)
{
    static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_ offset 0");
    static_assert(offsetof(InetAddress, addr_) == 0, "addr_ offset 0");
    if (ipv6)
    {
        memset(&addr6_, sizeof addr6_);
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = sockets::hostToNetwork16(port);
    }
    else
    {
        memset(&addr_, 0, sizeof addr_);
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackOnly > kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = sockets::hostToNetwork32(ip);
        addr_.sin_port = sockets::hostToNetwork16(port);
    }

}
