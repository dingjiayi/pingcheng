/*
// This is a public header file, it must only include public header files.
*/


#ifndef PC_NET_INETADDRESS_H
#define PC_NET_INETADDRESS_H

#include "base/string/StringPiece.h"
#include <netinet/in.h>

namespace pingcheng
{
namespace net
{
namespace sockets 
{
    const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
} // namespace sockets

// Wrapper of sockaddr_in.

// This is an POD interface class.
class InetAddress // copyable
{
public:
// Constructs an endpoint with given port number.
// Mostly used in TcpServer listening.

explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

// Constructs an endpoint with given ip and port.
// @C ip should be "1.2.3.4"
InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);

// Constructs an endpoint with given struct @C sockaddr_in
// Mostly used when accepting new connections
explicit InetAddress(const struct sockaddr_in& addr) : addr_(addr)
{}

explicit InetAddress(const struct sockaddr_in6& addr) : addr6_(addr)
{}

sa_family_t family() const { return addr_.sin_family; }
std::string toIp() const;
std::string toIpPort() const;
uint16_t toPort() const;

// default copy/ assignment are Okay
const struct sockaddr* getSockAddr() const 
{
    return sockets::sockaddr_cast(&addr6_);
}

void setSockAddrInet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }
 uint32_t ipNetEndian() const;
 uint16_t portNetEndian() const { return addr_.sin_port; }

 // resolve hostname to IP address, not chainging port or sin_family
 // return true on sucess.
 // thread safe
 static bool resolve(StringArg hostname, InetAddress* result);


private:
    union
    {
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    }
}
} // namespace net
} // namespace pingcheng

#endif // PC_NET_INETADDRESS_H