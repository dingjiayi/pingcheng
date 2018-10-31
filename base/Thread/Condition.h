
#ifndef PC_BASE_THREAD_CONDITION_H
#define PC_BASE_THREAD_CONDITION_H

#include "base/Thread/Condition.h"
#include <pthread.h>

namespace pingcheng
{

class Condition : noncopyable
{
public:
    explicit Condition(MutexLock& mutex) : mutex_(mutex)
    {
        MCHECK(pthread_cond_init(&pcond_, NULL));
    }

    ~Condition()
    {
        MCHECK(pthread_cond_destroy(&pcond_));
    }

    void wait()
    {
        MutexLock::UnassignGuard ug(mutex_);
        MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
    }

    // returns true if time out, false otherwise.
    bool waitForSeconds(double seconds);

    void notify()
    {
        MCHECK(pthread_cond_signal(&pcond_));
    }

    void notifyAll()
    {
        MCHECK(pthread_cond_broadcast(&pcond_));
    }
private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
}; // class Condition
} // namespace pingcheng

#endif // PC_BASE_THREAD_CONDITION_H