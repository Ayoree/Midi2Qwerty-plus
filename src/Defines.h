#define COLOR_WHITE IM_COL32_WHITE
#define COLOR_BLACK IM_COL32_BLACK

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}