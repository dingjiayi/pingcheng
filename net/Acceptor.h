
// This is an internal header file, you should not include this.
#ifndef PC_NET_ACCEPTOR_H
#define PC_NET_ACCEPTOR_H

#include <functional>

namespace pingcheng 
{
namespace net
{
class EventLoop;
class InetAddress;

/// 
/// Acceptor of incoming TCP connections.
///

class Acceptor : noncopyable
{
public:
    typedef std::function<void (int sockfd, const InetAddress& addr)> NewConnectionCallback;
    
} 


} // namespace pingcheng
} // namespace pingcheng


#endif // PC_NET_ACCEPTOR_H