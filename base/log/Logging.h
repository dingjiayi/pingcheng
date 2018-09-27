
/*
Taken from muduo : muduo/base/Logging.h
*/

#pragma once
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

}


}