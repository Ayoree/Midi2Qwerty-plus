#pragma once
#include <chrono>
#include "pch.h"
#include "Defines.h"

struct LogEntry
{
private:
    static constexpr inline std::array<const std::string_view, 4> s_typeToString = {"INFO", "ERROR", "WARNING", "DEBUG"};

public:
    enum class Type {
        INFO,
        ERROR,
        WARNING,
        DEBUG,
    };
    static inline const std::string_view typeToString(Type type) { return s_typeToString[static_cast<int>(type)]; }
    static inline const ImColor toColor(Type type)
    {
        switch(type)
        {
            case Type::INFO: return COLOR_WHITE;
            case Type::ERROR: return COLOR_RED;
            case Type::WARNING: return COLOR_YELLOW;
            case Type::DEBUG: return COLOR_GRAY;
            default: return COLOR_WHITE;
        }
    }

public:
    template <typename T>
    requires std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>
    explicit LogEntry(Type type, T&& message) :
        type(type),
        msg(std::forward<T>(message)),
        timestamp(std::chrono::system_clock::now()) 
    {}
    
    const std::string getTimeStr() const
    {
        using namespace std::chrono;

        time_t inTime = system_clock::to_time_t(timestamp);
        std::tm imBuf;
        localtime_s(&imBuf, &inTime);
        _V2::system_clock::duration since_epoch = timestamp.time_since_epoch();
        milliseconds ms = duration_cast<milliseconds>(since_epoch) % 1000;

        std::ostringstream timeSS;
        timeSS << std::put_time(&imBuf, "%H:%M:%S");

        return std::format("{}.{:03d}", timeSS.str(), ms.count());
    }

    inline std::string toString() const { return std::format("[{}][{}]\t{}", getTimeStr(), typeToString(type), msg); }
    
public:
    const Type type;
    const std::string msg;
    const std::chrono::system_clock::time_point timestamp;
};
