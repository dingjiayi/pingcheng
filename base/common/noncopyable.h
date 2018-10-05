#ifndef PINGCHENG_BASE_COMMON_NONCOPYABLE_H
#define PINGCHENG_BASE_COMMON_NONCOPYABLE_H

namespace pingcheng {
    namespace noncopyable_ {
    
        class noncopyable {
        protected:
            noncopyable() = default;
            ~noncopyable() = default;
            noncopyable(const noncopyable&) = delete;
            noncopyable& operator=(const noncopyable&) = delete;
        }
    }

    using noncopyable = noncopyable_::noncopyable;

}

#endif