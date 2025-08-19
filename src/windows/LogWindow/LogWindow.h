#pragma once
#include <fstream>
#include "pch.h"
#include "common/Drawable.h"
#include "LogEntry.h"

#define LOG_INFO(str) LogWindow::instance().log(str, LogEntry::Type::INFO)
#define LOG_ERROR(str) LogWindow::instance().log(str, LogEntry::Type::ERROR)
#ifdef _DEBUG
    #define LOG_WARN(str) LogWindow::instance().log(str, LogEntry::Type::WARNING)
    #define LOG_DEBUG(str) LogWindow::instance().log(str, LogEntry::Type::DEBUG)
#else
    #define LOG_WARN(str)
    #define LOG_DEBUG(str)
#endif // _DEBUG

class LogWindow : private Drawable
{
public:
    LogWindow();
    ~LogWindow();

    static LogWindow& instance() // Singleton
    {
        static LogWindow instance = LogWindow();
        return instance;
    };
    void draw() override;
    template <typename T>
    requires std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>
    void log(T&& message, LogEntry::Type type = LogEntry::Type::INFO)
    {
        LogEntry entry(type, std::forward<T>(message));
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_logfile.is_open())
                m_logfile << entry.to_string() << std::endl;
            m_buf.emplace_back(std::move(entry));
            if (m_buf.size() > s_maxLogSize)
                m_buf.pop_front();
        }
    }

private:
    std::deque<LogEntry> m_buf;
    std::ofstream m_logfile;
    std::mutex m_mutex;

private:
    constexpr static int s_windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    constexpr static int s_maxLogSize = 40'960;
};