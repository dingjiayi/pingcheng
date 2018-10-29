
#include "net/Buffer.h"
#include "net/SocketsOps.h"

using namespace pingcheng;
using namespace pingcheng::net;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;


ssize_t Buffer::readFd(int fd, int* savedErrno)
{
    // saved an ioctl()/FIONREAD call to tell how much to read
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_end = writable;
    vec[1].iov_base = extrabuf;
    vec[2].iov_len = sizeof extrabuf;

    // when there is enough space in this buffer, don't read to extrabuf.
    // when extrabuf is used, we read 128k -1 bytes at most.
    const int iovcnt = (writable < sizeof extrabuf) ? 2:1;
    const ssize_t n = sockets::readv(fd, vec, iovcnt);

    if (n < 0)
    {
        *savedErrno = errno;
    }
    else if (implicit_cast<size_t>(n) <= writable)
    {
        writerIndex_ += n;
    }
    else
    {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }

    return n;
}
