#pragma once

#define COLOR_WHITE ImColor(1.f, 1.f, 1.f, 1.f)
#define COLOR_BLACK ImColor(0.f, 0.f, 0.f, 1.f)
#define COLOR_GRAY ImColor(0.5f, 0.5f, 0.5f, 1.f)
#define COLOR_RED ImColor(1.f, 0.f, 0.f, 1.f)
#define COLOR_YELLOW ImColor(1.f, 1.f, 0.f, 1.f)
#define COLOR_BLUE ImColor(0.f, 0.f, 1.f, 1.f)

static inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x + b.x, a.y + b.y);
}