
#include "base/Logging.h"
#include "net/Channel.h"
#include "net/EventLoop.h"

#include <poll.h>
#include <sstream>

using namespace pingcheng;
using namespace pingcheng::net;

/// #define POLLIN		0x001		/* There is data to read.  */
/// #define POLLPRI		0x002		/* There is urgent data to read.  */
/// #define POLLOUT		0x004		/* Writing now will not block.  */


const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd__)
: loop_(loop),
  fd_(fd_),
  events_(0),
  revents_(0),
  index_(-1),
  logHup_(false),
  tied_(false),
  eventHandling_(false),
  addedToLoop_(false)
{
}

Channel::~Channel()
{
    assert(!eventHandling_);
    assert(!addedToLoop_);

    if (loop_ -> isInLoopThread())
    {
        assert(!loop_->hasChannel(this));
    }
}

void Channel::tie(const std::shared_ptr<void>& obj)
{
    tie_ = obj;
    tied_ = true;
}

void Channel::update()
{
    addedToLoop_ = true;
    loop_ ->updateChannel(this);
}

void Channel::remove()
{
    assert(isNonoEvent());
    addedToLoop_ = false;
    loop_ -> removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
    std::shared_ptr<void> guard;
    if (tied_)
    {
        guard = tie_.lock();
        if (guard)
        {
            handleEventWithGuard(receiveTime);
        }

    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    eventHandling_ = true;
    LOG_TRACE << reventsToString();
    if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
    {
        if (logHup_)
        {
            LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
        }
        if (closeCallback_) closeCallback_();
    }

    if (revents_ & POLLNVAL)
    {
        LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
    }

    if (revents_ & (POLLERR | POLLNVAL))
    {
        if (errorCallback_) errorCallback_();
    }

    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (readCallback_) readCallback_(receiveTime);
    }

    if (revents_ &  POLLOUT)
    {
        if (writeCallback_) writeCallback_()
    }

    eventHandling_ = false;
}

std::string Channel::reventsToString() const
{
    return eventsToString(fd_, revents_);
}

std::string Channel::eventsToString() const
{
    return eventsToString(fd_, events);
}

std::string Channel::eventsToString(int fd, int ev)
{
    std::ostringstream oss;

    oss << fd << ": ";
    if (ev & POLLIN)
        oss << "IN ";
    if (ev & POLLPRI)
        oss << "PRI ";
    if (ev & POLLOUT)
        oss << "OUT ";
    if (ev & POLLHUP)
        oss << "HUP ";
    if (ev & POLLERR)
        oss << "ERR ";
    if (ev & POLLNVAL)
        oss << "NVAL ";

    return oss.str().c_str();
}