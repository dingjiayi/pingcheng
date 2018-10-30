
#ifndef PC_BASE_THREAD_THREAD_H
#define PC_BASE_THREAD_THREAD_H

#include "base/common/noncopyable.h"
#include <cstdin>

namespace pingcheng
{
namespace detail
{
template <typename T>
class AtomicIntegerT : noncopyable 
{
public:
    AtomicIntegerT()
    : value_(0)
    {}

    T get()
    {
        // in gcc >= 4.7: 
        return __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
        //return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x)
    {
        // in gcc >= 4.7 
        return __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST);
        // return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x; 
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        decrementAndGet();
    }

    T getAndSet(T newValue)
    {
        // in gcc>= 4.7 
        return __atomic_exchange_n(&value, newValue, __ATOMIC_SEQ_CST);
        // return __sync_lock_test_and_set(&value_, newValue);
    }


private:
    volatile T value_;
}; // class AtomicIntegerT
} // detail

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
} // pingcheng 

#endif // PC_BASE_THREAD_THREAD_H