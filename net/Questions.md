## net 下面，一些疑惑

### 1. SocketsOps.cpp
1.1 toIpPort 函数中，为何直接将 struct sockaddr -> sockaddr_in,而不是 sockaddr_in6
1.2 在 getLocalAddr和getPeerAddr 中，都是sockaddr_in6 ，直接根据 in6->sin6_family去判断
这个需要求证

### 2. InetAddress
2.1 ipNetEndian()函数为何直接是AF_INET

### 3. Buffer.cpp
3.1 如果.cpp中不包含 
```
const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;
```
会如何。

### 4. Atomic.h
4.1 为何 addAndGet 不用 __atomic_add_fetch




---------------------------------------------------------
# 20181030 本周周末总结内容
1. C++ 11 内存模型
   参考地址：http://www.parallellabs.com/2011/08/27/c-plus-plus-memory-model/
   http://wiki.jikexueyuan.com/project/cplusplus-concurrency-action/  第五章
   https://www.ibm.com/developerworks/cn/linux/l-gcc-hacks/index.html
   http://blog.jobbole.com/106516/
   https://www.jianshu.com/p/da1d69f0a6ad
    内存栅栏：

2. __BEGIN_DECLS  ... __END_DECLS
    https://www.zhihu.com/question/27032678

3. __attribute__
   
   GNU C __attribute__ 机制简介
   http://blog.51cto.com/afreez/7351
   http://www.keil.com/support/man/docs/armcc/armcc_chr1359124976881.htm
   
