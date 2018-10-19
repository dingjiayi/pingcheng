
#include "base/Logging.h"
#include "net/Channel.h"
#include "net/EventLoop.h"

#include <poll.h>

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
        assert()
    }
}