
// This is a public header file, it must only include public header files.

#ifndef PC_NET_EVENTLOOP_H
#define PC_NET_EVENTLOOP_H

#include <vector>

namespace pingcheng
{
namespace net
{
class Channel;
class Poller;
class TimerQueue;

///
/// Reactor, at most one per thread.
///
/// This is an interface class, so don't expose too much details.

class EventLoop : noncopyable
{
public:

private:
    void abortNotInLoopThread();
    void handleRead(); // waked up
    void doPendingFunctors();

    void printActiveChannels() const; // DEBUG

    typedef std::vector<Channel*> ChannelList;

    bool looping_; /* atomic */
    std::atomic<bool> quit_;
    bool eventHandling_; /* atomic */
    bool callingPendingFunctors_; /* atomic */
    int64_t iteration_;
    const pid_t threadId_;
    Timestamp poolReturnTime_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;

    // unlike in TimerQueue, Which is an internal class,
    // we don't expose Channel to client.
    std::unique_ptr<Channel> wakeupChannel_;
    
}

}
}


#endif // PC_NET_EVENTLOOP_H