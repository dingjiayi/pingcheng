
#ifndef PC_NET_SOCKET_H
#define PC_NET_SOCKET_H

#include "base/common/noncopyable.h"

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

namespace pingcheng
{
// TCP networking
namespace net
{
class InetAddress;

// Wrapper of socket file descriptor.
// It closes the sockfd when destructs.
// It's thread safe, all operations are dalagated to OS.


} // namespace net
} // namespace pingcheng

#endif