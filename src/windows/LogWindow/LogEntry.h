#pragma once
#include <chrono>
#include "pch.h"
#include "Defines.h"

struct LogEntry
{
public:
    enum class Type {
        INFO,
        ERROR,
        WARNING,
        DEBUG,
    };
    static const std::string& to_string(Type type) {
        return s_typeToString[static_cast<int>(type)];
    }
    static const ImColor to_color(Type type) {
        switch(type)
        {
            case Type::INFO: return COLOR_WHITE;
            case Type::ERROR: return COLOR_RED;
#ifdef _DEBUG
            case Type::WARNING: return COLOR_YELLOW;
            case Type::DEBUG: return COLOR_GRAY;
#endif // _DEBUG
            default: return COLOR_WHITE;
        }
    }
private:
    constexpr static std::array<std::string, 4> s_typeToString = {"INFO", "ERROR", "WARNING", "DEBUG"};

public:
    template <typename T>
    requires std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>
    explicit LogEntry(Type type, T&& message) :
        type(type),
        msg(std::forward<T>(message)),
        timestamp(std::chrono::system_clock::now()) 
    {}
    
    std::string get_time() const {
        using namespace std::chrono;

        time_t inTime = system_clock::to_time_t(timestamp);
        std::tm imBuf;
        localtime_s(&imBuf, &inTime);
        _V2::system_clock::duration since_epoch = timestamp.time_since_epoch();
        milliseconds ms = duration_cast<milliseconds>(since_epoch) % 1000;
    
        std::ostringstream timeSS;
        timeSS << std::put_time(&imBuf, "%H:%M:%S");

        // Форматируем через std::format
        return std::format("{}.{:03d}", timeSS.str(), ms.count());
    }

    std::string to_string() const {
        return "[" + get_time() + "][" + to_string(type) + "]\t" + msg;
    }
    
    const Type type;
    const std::string msg;
    const std::chrono::system_clock::time_point timestamp;
};
