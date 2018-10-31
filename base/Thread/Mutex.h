
#ifndef PC_BASE_MUTEX_H
#define PC_BASE_MUTEX_H

#include "base/Thread/CurrentThread.h"
#include "base/Common/noncopyable.h"

#include <cassert>
#include <pthread.h>

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG

__BEGIN_DECLS

extern void __assert_perror_fail ( int errnum,
                                   const char *file,
                                   unsigned int line,
                                   const char *function
                                )
    __THROW __attribute__ ((noreturn));

__END_DECLS

#endif //  NDEBUG

#define MCHECK(ret) (                           \
        { __typeof__ (ret) errnum = (ret);
            if (__builtin_expect(errnum != 0, 0)) \
                __assert_perror_fail(errnum, __FILE__, __LINE__, __func__); \
        }            \
)

#else   // CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({__typeof__ (ret) errnum = (ret);  \
                     assert(errnum == 0); (void) errnum; \
})

#endif // CHECK_PTHREAD_RETURN_VALUE

namespace pingcheng 
{
class MutexLock : noncopyable
{
public:
    MutexLock() : holder_(0)
    {
        MCHECK(pthread_mutex_init(&mutex_, NULL));
    }

    ~MytexLock()
    {
        assert(holder_ == 0);
        MCHECK(pthread_mutex_destroy(&mutex_));
    }

    // must be called when locked, i.e. for assertion
    bool isLockedByThisThread() const
    {
        return holder_ == CurrentThread::tid();
    }

    void assertLocked() const 
    {
        assert(isLockedByThisThread());
    }

    // internal usage
    void lock()
    {
        MCHECK(pthread_mutex_lock(&mutex_));
        assignHolder();
    }

    void unlock()
    {
        unassignHolder();
        MECHECK(pthread_mutex_unlock(&mutex_));
    }

    pthread_mutex_t* getPthreadMutex() /* non-const */
    {
        return &mutex_;
    }

private:
    friend class Condition;
    class UnassignGuard : noncopyable
    {
    public:
        UnassignGuard(MutexLock& owner) : owner_(owner)
        {
            owner_.unassignHolder();
        }

        ~UnassignGuard()
        {
            owner_.assignHolder();
        }

    private:
        MutexLock& owner_;
    }; // class UnassignGuard

    void unassignHolder()
    {
        holder_ = 0;
    }

    void assignHolder()
    {
        holder_ = CurrentThread::tid();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;

}; // class MutexLock

class MutexLockGuard : noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:
    MutexLock& mutex_;
} // class MutexLockGuard

} // namespace pingcheng

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif  // PC_BASE_MUTEX_H