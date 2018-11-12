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
   

4. MIT 分布式系统 
   https://pdos.csail.mit.edu/6.824/
   https://www.zhihu.com/question/24750289
   https://yq.aliyun.com/articles/8069

--------------------------------------------------------------------
1. 指针和引用
   1.1. sizeof 相等
   1.2. 取地址相等
   1.3. 指针和引用的区别
   1.4. 参数传递 引用比指针好
   1.5. 常见用途： 参数类型和返回类型
   1.6. 函数后面的const 是函数前面的一部分
   

   ----------------------------------继承、组合关系下的构造和硒鼓-----------------------------------
   1. 继承 构造由内而外  析构由外而内
   2. 组合 同上
   3. 复合 构造由内而外  析构由外而内  但是内部顺序怎么排？
   
   --------------------------------------- vptr vtbl  -------------------------------------------
   1. A B C   B 的指针，转化为A的，然后调用虚函数，这个n怎么确定？
   2. 符合哪三个条件，动态绑定？ 指针 虚函数 指针upcast
   3. https://www.ibm.com/developerworks/cn/java/j-lo-polymorph/index.html
   4. https://zhuanlan.zhihu.com/p/37445001
   5. 虚函数绝大部分用于：多态 模板方法 Template Method
   6. https://www.jianshu.com/p/4a65e84559db

   --------------------------------------- this  -------------------------------------------
    1. C++ 类的成员函数有隐含的擦书 this


   --------------------------------------- 动态绑定 Dynamic Binding  -------------------------------------------
   1. 通过对象来调用，静态绑定 static binding
   2. https://blog.csdn.net/Richie_ll/article/details/80375549


    ------------------------------------ 陈硕 TCP连接 ------------------------------------
    1. https://zhuanlan.zhihu.com/p/25241630
    2. https://blog.csdn.net/solstice/article/details/6579232

    ---------------------------------------- 编译连接问题 ----------------------------------------
    Linux 动态库与静态库制作及使用详解  https://www.ibm.com/developerworks/cn/linux/l-cn-linklib/index.html

    ---------------------------------------- skynet ----------------------------------------
    https://blog.csdn.net/vonzhoufz/article/details/38877337
