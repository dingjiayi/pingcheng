
#include "net/SocketsOps.h"
#include "base/Logging.h"
#include "net/Endian.h"

using namespace pingcheng;
using namespace pingcheng::net;

namespace 
{
using SA = struct sockaddr;

#if VALGRIND || defined (NO_ACCEPT4)
void setNonBlockAndCloseOnExec(int sockfd)
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
        LOG_SYSFATAL << "sockets::createNonblockingOrDie 1" << sockfd;
    }

    setNonBlockAndCloseOnExec(sockfd);
#else
    int sockfd = socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "sockets::createNonblockingOrDie 2";
    }
#endif

    return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr* addr)
{
    int ret = :: bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    if (ret < 0)
    {
        LOG_SYSFATAL << "sockets::bindOrDie 1";
    }

}

void sockets::listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0)
    {
        LOG_SYSFATAL << "sockets::listenOrDie";
    }
}

int sockets::accept(int sockfd, struct sockaddr_in6* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
#if VALGRIND || defined (NO_ACCEPT4)
    int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
    setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = ::accpet4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif

    if (connfd < 0)
    {
        int savedErrno = errno;
        LOG_SYSERR << "Socket::accept";

        switch (savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO: 
            case EPRERM:
            case EMFILE:
             // expected errors
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                LOG_FATAL << "unexpected error of ::accept " << savedErrno;
                break;
            default:
                LOG_FATAL << "unknow error of ::accept " << savedErrno;
                break;
        } // switch
    } // if

    return connfd;
}


