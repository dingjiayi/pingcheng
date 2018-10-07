
#ifndef PC_BASE_LOG_LOGSTREAM_H
#define PC_BASE_LOG_LOGSTREAM_H

#include "base/string/StringPiece.h"
#include "base/common/noncopyable.h"

#include <cstring>

namespace pingcheng 
{
namespace detail
{
    const int kSmallBuffer = 4000;
    const int kLargeBuffer = 4000 * 1000;

template<int SIZE>
class FixedBuffer : pingcheng::noncopyable 
{
public:
    FixedBuffer() : cur_(data_)
    {
        setCookie(cookieStart);
    }

    ~FixedBuffer()
    {
        setCookie(cookieEnd);
    }

    void append(const char* /*restrict*/ buf, size_t len)
    {
        // FIXME: append partially
        if (static_cast<size_t>(avail()) > len)
        {
            std::memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    const char* data() const { return data_; }
    int length() const { return static_cast<int>(cur_ - data_); }

    // write to data_ directly
    char* curent() { return cur_; }
    int avail() const { return static_cast<int>(end() - cur_); }
    void add(size_t len) { cur_ += len; }

    void reset() { cur_ = data_; }
    void bzero() { memset(data_, 0, sizeof data_); }

    // for used by GDB
    const char* debugString();
    void setCookie(void (*cookie)()) { cookie_ = cookie; }
    // for used by unit test
    std::string toString() const { return std::string(data_, length() ); }
    StringPiece toStringPiece() const { return StringPiece(data_, length() ); }

private:
    const char* end() const { return data_ + sizeof data_;}

    // Must be outline function for cookies
    static void cookieStart();
    static void cookieEnd();

    void (*cookie_)();
    char data_[SIZE];
    char* cur_;

} // class FixedBuffer

} // namespace detail

class LogStream : pingcheng::noncopyable 
{
public:
    using self = LogStream;
    using Buffer = detail::FixedBuffer<detail::kSmallBuffer> ; 

    self& operator<<(bool v)
    {
        buffer_.append(v ? "1" : "0", 1);
    }   

    self& operator<<(short);
    self& operator<<(unsigned short);
    self& operator<<(int);
    self& operator<<(unsigned int);
    self& operator<<(long);
    self& operator<<(unsigned long);
    self& operator<<(long long);
    self& operator<<(unsigned long long);
    self& operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }

    self& operator<<(char v)
    {
        buffer_.append(&v, 1);
        return *this;
    }

    self& operator<<(const char* str)
    {
        if (str)
        {
            buffer_.append(str, strlen(str));
        }
        else
        {
            buffer_.append("(null)", 6);
        }

        return *this;
    }

    self& operator<<(const unsigned char* str)
    {
        return operator<<(reinterpret_cast<const char*>(str));
    }

    self& operator<<(const std::string& v)
    {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    self& operator<<(const StringPiece& v)
    {
        buffer_.append(v.data(), v.size());
        return *this;
    }

    void append(const char* data, int len) { buffer_.append(data, len); }
    const Buffer& buffer() const { return buffer_; }
    void resetBuffer() { buffer_.reset(); }

private:
    void staticCheck();

    template<typename T> void formatInteger(T);

    Buffer buffer_;

    static const int kMaxNumericSize = 32;

}

class Fmt // noncopyable
{
public:
    template<typename T> Fmt(const char& fmt, T val);

    const char* data() const { return buf_; }
    int length() const { return length_; }
private:
    char buf_[32];
    int length_;
}

inline LogStream& operator<<(LogStream& s, const Fmt& fm)
{
    s.append(fmt.data(), fmt.length());
    return s;
}

} // namespace pingcheng 

#endif  //PC_BASELOG_LOGSTREAM_H