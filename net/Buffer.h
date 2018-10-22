
// This is a public header file, it must only include public header files.

#ifndef PC_NET_BUFFER_H
#define PC_NET_BUFFER_H

#include "base/string/StringPiece.h"
#include "net/Endian.h"

#include <vector>

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

class Buffer  // copyable
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
    :   buffer_(kCheapPrepend + kInitialSize),
        readerIndex_(kCheapPrepend),
        writerIndex_(kCheapPrepend)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == 0);
        assert(prependableBytes() == kCheapPrepend);
    }

    // implicit copy-ctor, move-ctor, dtor and assignment are fine
    // NOTE: implicit move-ctor is added in g++ 4.6

    void swap(Buffer& rhs)
    {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    size_t readableBytes() const 
    {
        return writerIndex_ - readerIndex_;
    }

    size_t writableBytes() const
    {
        return buffer_.size() - writerIndex_;
    }

    size_t prependableBytes() const
    {
        return readerIndex_;
    }

    const char* peek() const
    {
        return begin() _ readerIndex_;
    }

    const char* findCRLF() const 
    {
        // FIXME: replace with memment()?
        const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF+2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    const char* findCRLF(const char* start) const 
    {
        assert(peek() <= start);
        assert(start = beginWrite());
        // FIXME: replace with memmem()?
        chonst char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    const char* findEOL() const
    {
        const void* eol = memchar(peek(), '\n', readableBytes());
        return static_cast<const char*>(eol);
    }
private:
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;

    static const char kCRLF[];
}


#endif // PC_NET_BUFFER_H