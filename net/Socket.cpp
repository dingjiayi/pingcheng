
#include "net/Socket.h"
#include "base/Logging.h"
#include "net/SocketsOps.h"

using namespace pingcheng;
using namespace pingcheng::net;

Sockets::~Socket()
{
    sockets::close(sockfd_);
}

/// struct tcp_info is in <netinet/tcp.h>
bool Socket::getTcpInfo(struct tcp_info* tcpi) const
{
    memset(&tcpi, 0, sizeof *tcpi));
    return ::getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

// 各个字段含义可以查看文章 https://blog.csdn.net/zhangskd/article/details/8561950
bool Socket::getTcpInfoString(char* buf, int len) const
{
    struct tcp_info tcpi;
    bool ok = getTcpInfo(&tcpi);
    if (ok)
    {
        snprintf(buf, len, "unrecovered=%u  "
                "rto=%u ato=%u snd_mss=%u rcv_mss=%u"
                "lost=%u retrans=%u rtt=%u rttvar=%u"
                "sshthresh=%u cwnd=%u total_retrans=%u",
                tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
                tcpi.tcpi_rto,          // Retransmit timeout in usec
                tcpi.tcpi_ato,           // Predicted tick of soft clock in usec
                tcpi.tcpi_snd_mss,      
                tcpi.tcpi_rcv_mss, 
                tcpi.tcpi_lost,         // Lost packets
                tcpi.tcpi_retrans,      // Restransmitted packets out
                tcpi.tcpi_rtt,          // Smoothed round trip time in usec
                tcpi.tcpi_rttvar,       // Medium deviation
                tcpi.tcpi_snd_ssthresh,
                tcpi.tcpi_snd_cwnd,
                tcpi.tcpi_total_retrans  // Total retransmits for entire connection
        );
    }  // if ok

    return ok;
}

void Socket::bindAddress(const InetAddress& addr)
{
    sockets::bindOrDie(sockfd_, addr.getSockAddr());
}

void Socket::listen()
{
    sockets::listenOrDie(sockfd_);
}

int Socket::accpet(InetAddress* peeraddr)
{
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof addr);
    int connfd = sockets::accept(sockfd_, &addr);
    if (connfd >=0)
    {
        peeraddr->setSockAddrInet6(addr);
    }

    return connfd;
}

void Socket::shutdownWrite()
{
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1: 0;
    :: setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
    // FIXME CHECK
}

void Socket::setReuseAddr(bool on)
{
    int optval = on? 1: 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));

    // FIXME CHECK
}

void Socket::setReusePort(bool)
{
#ifdef SO_REUSEPORT
    int optval = on ? 1: 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval))
    if (ret < 0 && on)
    {
        LOG_SYSERR << "SO_REUSEPORT failed.";
    }
#else
    if (on)
    {
        LOG_ERROR << "SO_REUSEPORT is not supported.";
    }
#endif
}

void Socket::setKeepAlive(bool on )
{
    int optval = on ? 1:0;
    :: setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval));

    // FIXME CHECK
}