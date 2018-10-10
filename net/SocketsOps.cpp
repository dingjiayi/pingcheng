
#include "net/SocketsOps.h"
#include "base/Logging.h"
#include "net/Endian.h"

using namespace pingcheng;
using namespace pingcheng::net;

namespace 
{
using SA = struct sockaddr;

#if VALGRIND || defined (NO_ACCEPT4)
void setNonBlockAndCloseOnExc(int sockfd)
{
    // non-block
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);
    // to optimize ： chekc ret err
    // close-on-exec
    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags := FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flags);
    // to optimize
    (void)ret;

}
#endif    
} // namesapce

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in6* addr)
{
    return reinterpret_cast<const struct sockaddr*>(addr);
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6* addr)
{
    return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
    return reinterpret_cast<const struct sockaddr*>(addr);
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in* addr)
{
    return reinterpret_cast<struct sockaddr*>(addr);
}

const struct sockaddr* sockets::sockaddr_in_cast(const struct sockaddr_in* addr)
{
    return reinterpret_cast<const struct sockaddr*>(addr);
}

const struct sockaddr* sockets::sockaddr_in6_cast(const struct sockaddr_in6* addr)
{
    return reinterpret_cast<const struct sockaddr*>(addr);
}

int sockets::createNonblockingOrDie(sa_family_t family)
{
#if VALGRIND
    int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "sockets::createNonblockingOrDie" << sockfd;
    }
#else

#endif
}