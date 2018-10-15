
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
    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    {
        newConnectionCallback_ = cb;
    }

    void listenning_() const { return listenning_;}
    void listen();
private:
    void handleRead();
    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
    int idleFd_;
};


} // namespace pingcheng
} // namespace pingcheng


#endif // PC_NET_ACCEPTOR_H