
// This is a public header file, it must only include public header files.

#ifndef PC_NET_BUFFER_H
#define PC_NET_BUFFER_H

#include "base/string/StringPiece.h"
#include "net/Endian.h"

#include <vector>
#include <algorithm>

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

    const char* findEOL(const char* start ) const 
    {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const void* eol = memchar(start, '\n', beginWrite() - start);
        return static_cast<const char*>(eol);
    }

    // retrieve returns void, to prevent
    // string str(retrieve(readableBytes()), readableBytes());
    // the evaluation of two functions ars unspecified
    void retrieve(size_t len)
    {
        assert(len <= readableBytes());
        if (len < readableBytes())
        {
            readerIndex_ += len;
        }
        else
        {
            retrieveAll();
        }
    }

    void retrieveUntil(const char* end)
    {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve(end - peek());
    }

    void retrieveInt64()
    {
        retrieve(sizeof(int64_t));
    }

    void retrieveInt32()
    {
        retrieve(sizeof(int32_t));
    }

    void retrieveInt16()
    {
        retrieve(sizeof(int16_t));
    }
    void retrieveInt8()
    {
        retrieve(sizeof(int8_t));
    }

    void retrieveAll()
    {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    std::string retrieveAsString(size_t len)
    {
        assert(len <= readableBytes());
        std::string result(peek(), len);
        retrieve(len);
        return result;
    }

    StringPiece toStringPiece() const 
    {
        return StringPiece(peek(), static_cast<int>(readableBytes()));
    }

    void append(const StringPiece& str)
    {
        append(str.data(), str.size());
    }

    void append(const char* /*restrict*/ data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data + len, beginWrite());
        hasWritten(len);
    }

    v
private:
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;

    static const char kCRLF[];
}


#endif // PC_NET_BUFFER_H