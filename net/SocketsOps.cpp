
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


int sockets::connect(int sockfd, const struct sockaddr* addr)
{
    return connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6));)
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd)
{
    ::close(sockfd);
}

void sockets::shutdownWrite(int sockfd)
{
    if (::shutdownWrite(sockfd, SHUT_WR) < 0)
    {
        LOG_SYSERR << "sockets::shutdownWrite";
    }
}

void sockets::toIp(char* buf, size_t size, const sockaddr* addr)
{
    if (addr->sa_family == AF_INET)
    {
        assert(size >= INET_ADDRSTRLEN);
        const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
        ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
    }
    else if (addr->sa_family == AF_INET6)
    {
        assert(size >= INET6_ADDRSTRLEN);
        const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
        ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
    }
}

void sockets::toIpPort(char* buf, size_t size, const struct sockaddr* addr)
{
    toIp(buf, size, addr);
    size_t end = ::strlen(buf);
    const struct sockaddr_in* addr4 = sockaddr_in_cast(addr);
    uint16_t port = sockets::networkToHost16(addr4->sin_port);
    assert(size > end);
    snprintf(buf + end, size - end, ":%u", port);
}


void sockets::fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = hostToNetwork16(port);
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
    {
        LOG_SYSERR << "sockets::fromIpPort ipv4";
    }
}

void sockets::fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr)
{
    addr->sin6_family = AF_INET6;
    addr->sin6_port = hostToNetwork16(port);
    if (::inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0)
    {
        LOG_SYSERR << "sockets::fromIpPort ipv6";
    }
}

int sockets::getSocketError(int sockfd)
{
    int optval;
    socklent_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        return errno;
    }
    else
    {
        return optval;
    }
}