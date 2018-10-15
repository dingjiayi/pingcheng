
// This is an internal header file, you should not include this.

#ifndef PC_NET_CHANNEL_H
#define PC_NET_CHANNEL_H

#include "base/Timestamp.h"

#include <functional>

namespace pingcheng
{
namespace net
{
class EventLoop;

///
/// A selectable I/O channel.
/// 
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd



} // namespace net
} // namespace pingcheng

#endif //PC_NET_CHANNEL_H