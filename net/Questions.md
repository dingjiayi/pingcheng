## net 下面，一些疑惑

### 1. SocketsOps.cpp
1.1 toIpPort 函数中，为何直接将 struct sockaddr -> sockaddr_in,而不是 sockaddr_in6
1.2 在 getLocalAddr和getPeerAddr 中，都是sockaddr_in6 ，直接根据 in6->sin6_family去判断
这个需要求证

### 2. InetAddress
2.1 ipNetEndian()函数为何直接是AF_INET
