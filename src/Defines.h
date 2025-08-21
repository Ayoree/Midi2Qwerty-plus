#pragma once

#include "pch.h"

#define COLOR_TRANSPARENT ImColor(0, 0, 0, 0)
#define COLOR_WHITE ImColor(1.f, 1.f, 1.f, 1.f)
#define COLOR_BLACK ImColor(0.f, 0.f, 0.f, 1.f)
#define COLOR_GRAY ImColor(0.5f, 0.5f, 0.5f, 1.f)
#define COLOR_RED ImColor(1.f, 0.f, 0.f, 1.f)
#define COLOR_YELLOW ImColor(1.f, 1.f, 0.f, 1.f)
#define COLOR_BLUE ImColor(0.f, 0.f, 1.f, 1.f)
#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2

constexpr std::string_view APP_NAME = "MIDI 2 QWERTY plus";
constexpr std::string_view LOG_FILE_PATH = "log.txt";

static inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x + b.x, a.y + b.y);
}
static inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x - b.x, a.y - b.y);
}

namespace std 
{
    template <typename T>
    using uptr = unique_ptr<T>;
    template <typename T>
    using sptr = shared_ptr<T>;
    template <typename T>
    using wptr = weak_ptr<T>;
};
namespace make
{
    template <typename T, typename... Args>
    std::unique_ptr<T> uptr(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    template <typename T, typename... Args>
    std::unique_ptr<T> sptr(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

template<typename T> 
struct Vec2
{
    union {
        struct { T x, y; };
        struct { T a, b; };
        struct { T u, v; };
        struct { T width, height; };
        std::array<T, 2> data;
    };

    constexpr Vec2() : x(T()), y(T()) {}
    
    // From arithmetic scalar
    template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    constexpr explicit Vec2(U scalar) : x(static_cast<T>(scalar)), y(static_cast<T>(scalar)) {}
    
    // From two arithmetic scalars
    template<typename U, typename V, typename = std::enable_if_t<std::is_arithmetic_v<U> && std::is_arithmetic_v<V>>>
    constexpr Vec2(U x, V y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}
    
    // From Vec2
    constexpr explicit Vec2(const Vec2& other) : x(other.x), y(other.y) {}
    
    // From ImVec2
    constexpr explicit Vec2(const ImVec2& imVec) : x(static_cast<T>(imVec.x)), y(static_cast<T>(imVec.y)) {}

    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    Vec2 operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }
    Vec2 operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }
    Vec2& operator*=(const Vec2& other) { x *= other.x; y *= other.y; return *this; }
    Vec2& operator/=(const Vec2& other) { x /= other.x; y /= other.y; return *this; }
    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vec2& other) const { return !(*this == other); }
    bool operator<(const Vec2& other) const { return this->length() < other->length(); }
    bool operator<=(const Vec2& other) const { return this->length() <= other->length();; }
    bool operator>(const Vec2& other) const { return !(*this <= other); }
    bool operator>=(const Vec2& other) const { return !(*this < other); }

    T& operator[](uint8_t index) { return data[index]; }
    const T& operator[](uint8_t index) const { return data[index]; }

    operator ImVec2() const { return ImVec2(static_cast<float>(x), static_cast<float>(y)); }

    T length() const {
        return std::sqrt(x * x + y * y);
    }
    Vec2 normalize() const {
        T len = length();
        return len != 0 ? *this / len : *this;
    }
    Vec2 abs() const {
        return Vec2(std::abs(x), std::abs(y));
    }
};

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;
