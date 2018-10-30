
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
        // in gcc >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    
private:
    volatile T value_;
} // class AtomicIntegerT
} // detail
} // pingcheng 

#endif // PC_BASE_THREAD_THREAD_H