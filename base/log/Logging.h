
/*
Taken from muduo : muduo/base/Logging.h
*/

#ifndef PC_BASE_LOG_LOGGING_H
#define PC_BASE_LOG_LOGGING_H

namespace pingcheng 
{
class TimeZone;
class Logger
{
public:
    enum LogLevel 
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    // compile time calculation of basename of source file
    class SourceFile
    {
        public:
            template<int N>
            inline SourceFile(const char (&attr)[N]) : data_(arr), size_(N-1);
            {
                const char* slash = strrchr(data_, '/');
                if (slash)
                {
                    data_ = slash +1;
                    size_ -= static_cast<int>(data -arr);
                }
            }

            explicit SourceFile(const char* filename) : data_(filename)
            {
                const char* slash = strrchr(filename, '/');
                if (slash)
                {
                    data_ = slash + 1;
                }

                size_ = static_cast<int>(strlen(data_));
            }

            const char* data_;
            int size_;
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

    LogStream& stream() { return impl_.stream_;}

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void(*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);
    static void setTimeZone(const TimeZone& tz);

private:

    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;
        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
        void formatTime();
        void finish()

        Timestamp time_;
        LogStream stream_;
        LogLevel level_;
        int line_;
        SourceFile basename_;
    };

    Impl impl_;

}; // Logger

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
    return g_logLevel;
}

/* CAUTION: do not write:
// if (good)
// LOG_INFO << "Good news";
// else
// LOG_WARN << "Bad news";
// 
// this expends to 
// 
// if (good)
//    if (logging_INFO)
//         logInfoStream << "Good news";
//    else 
//          logWarnStream << "Bad news";

*/

#define LOG_TRACE if (pingcheng::Logger::logLevel() <= pingcheng::Logger::TRACE) \
    pingcheng::Logger(__FILE__, __LINE__, pingcheng::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (pingcheng::Logger::logLevel() <= pingcheng::Logger::DEBUG) \
    pingcheng::Logger(__FILE__, __LINE__, pingcheng::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (pingcheng::Logger::logLevel() <= pingcheng::Logger::INFO) \
    pingcheng::Logger(__FILE__, __LINE__, pingcheng::Logger::INFO, __func__).stream()
#define LOG_WARN pingcheng::Logger(__FILE__, __LINE__, pingcheng::Logger::WARN, __func__).stream()
#define LOG_ERROR pingcheng::Logger(__FILE__, __LINE__, pingcheng::Logger::ERROR).stream()
#define LOG_FATAL pingcheng::Logger(__FILE__, __LINE__, pingcheng::Logger::FATAL, __func__).stream()
#define LOG_SYSERR pingcheng::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL pingcheng::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);
// Taken from glog/logging.h
//
// Check that the input is non NULL. This is very useful in constructor initializer lists.
#define CHECK_NOTNULL(val) \
    ::pingcheng::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helpter for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char* names, T* ptr)
{
    if (ptr == nullptr)
    {
        Logger(file, line, Logger::FATAL).stream() << names;
    }

    return ptr;
}

}

#endif