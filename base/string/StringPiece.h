/*
@taken from muduo : muduo/base/StringPiece.h
*/

#pragma once

namespace pingcheng {
// for passing c-style string argument to a function
class StringArg // copyable
{
public:
    StringArg(const char* str): str_(str)
    {}
    StringArg(const std::string& str): str_(str.c_str())
    {}

    const char* c_str() {return str_; }
private:
    const char* str_;

};

class StringPiece {
public:
    // we provide non-explicit singleton constructors so users 
    // can pass in a "const char*" or a "string" wherever a "StringPiece" is expected
    StringPiece() : ptr_(nullptr), length_(0) {}
    StringPiece(const char* str) : ptr_(str), length_(static_cast<int>(strlen(ptr_))) {}
    StringPiece(const unsigned char* str) 
    : ptr(reinterpret_cast<const char*>(str)), 
      length_(static_cast<int>(strlen(ptr_))
    {}

    StringPiece(const std::string& str): ptr(str.data()), length_(static_cast<int>(str.size())) {}
    StringPiece(const char* offset, int len): ptr_(offset), length_(len) {}

    const char* data() const {return ptr_; }
    int size() const { return length_; }
    bool empty() const { return length_ == 0; }
    const char* begin() const { return ptr_; }
    const char* end() const { return ptr_ + length_; }

    void clear() { ptr_ = nullptr; length_ = 0; }
    void set(const char* buffer, int len) { ptr_ = buffer; length_ = len; }
    void set(const char* str) { ptr_ = str; length_ = static_cast<int>(strlen(str)); }
    void set(const void* buffer, int len) { ptr_ = reinterpret_cast<const char*>(buffer); length_ = len;}

    char operator[](int i) const { return ptr_[i]; }
    void remove_prefix(int n) 
    {
        ptr_ += n;
        length_ -= n;
    }
    void remove_suffix(int n) {
        length_ -= n;
    }

    bool operator==(const StringPiece& x) const {
        return (length_ == x.length_  && (memcmp(ptr_, x.ptr_, length_) == 0)); 
    }
    bool operator!= (const StringPiece& x) const { return !(*this == x); }

    #define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)           \
        bool operator cmp (const StringPiece& x) const {         \
            int r = memcmp(ptr_, x.ptr_, (length_ < x.length_) ? length_ : x.length; )  \
            return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));             \
        }                                                                               \

        STRINGPIECE_BINARY_PREDICATE(<, <);
        STRINGPIECE_BINARY_PREDICATE(<=, <);
        STRINGPIECE_BINARY_PREDICATE(>=, >);
        STRINGPIECE_BINARY_PREDICATE(>, >);
    
    #undef STRINGPIECE_BINARY_PREDICATE
private:
    const char* ptr_;
    int length_;
};
} // namespace pingcheng


