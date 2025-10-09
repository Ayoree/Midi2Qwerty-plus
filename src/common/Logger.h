#pragma once
#include "pch.h"
#include "LogEntry.h"

#define LOG_INFO(str) Logger::instance().log(str, LogEntry::Type::INFO)
#define LOG_ERROR(str) Logger::instance().log(str, LogEntry::Type::ERROR)
#ifdef _DEBUG
    #define LOG_WARN(str) Logger::instance().log(str, LogEntry::Type::WARNING)
    #define LOG_DEBUG(str) Logger::instance().log(str, LogEntry::Type::DEBUG)
#else // Do not log warnings and debugs in release build
    #define LOG_WARN(str)
    #define LOG_DEBUG(str)
#endif // _DEBUG

class Logger
{    
private:
    static constexpr inline int s_maxLogSize = 40'960;

private:
    // Singleton
    Logger() = default;
    ~Logger() = default;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger& instance()
    {
        static Logger instance = Logger();
        return instance;
    };

    const std::deque<LogEntry>& getBuffer() const { return m_buf; };

    template <typename T>
    requires std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>
    void log(T&& message, LogEntry::Type type = LogEntry::Type::INFO)
    {
        LogEntry entry(type, std::forward<T>(message));
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::cout << entry.toString() << std::endl;
            m_buf.emplace_back(std::move(entry));
            if (m_buf.size() > s_maxLogSize)
                m_buf.pop_front();
        }
    }

private:
    std::deque<LogEntry> m_buf;
    std::mutex m_mutex;
};