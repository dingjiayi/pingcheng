
// This is a public header file, it must only include public header files.

#ifndef PC_NET_BUFFER_H
#define PC_NET_BUFFER_H

#include "base/StringPiece.h"
#include "net/Endian.h"

namespace pingcheng 
{
namespace net
{
/// A buffer class modeled aafter org.jboss.netty.buffer.ChannelBUffer
/// 
/// @code 
/// +---------------------+---------------------+---------------------+
/// | prependable bytes   | readable bytes      | writable bytes      |
/// |                     |     (CONTENT)       |                     |
/// +---------------------+---------------------+---------------------+
/// |                     |                     |                     |
/// 0       <=       readerIndex    <=     writerIndex        <=     size
/// +---------------------+---------------------+---------------------+
/// @endcode

class BUffer  // copyable
{
    
}


#endif // PC_NET_BUFFER_H