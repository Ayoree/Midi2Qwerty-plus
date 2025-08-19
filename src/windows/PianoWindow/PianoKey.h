#pragma once
#include "pch.h"
#include "common/Drawable.h"

class PianoKey : private Drawable
{
public:
    explicit PianoKey(uint8_t keyId);
    ~PianoKey();

    void draw() override;

    bool isBlack() const { return m_isBlack; }
    uint8_t getIndex() const { return m_id; }
    float getWidth() const { return m_isBlack ? s_blackSize.x : s_whiteSize.x; }
    void setPos(ImVec2 pos) { m_pos = pos; }

private:
    const uint8_t m_id = 0;
    const bool m_isBlack = false;
    ImVec2 m_pos = {0, 0};

public:
    constexpr static ImVec2 s_whiteSize = {18.f, 120.f};
    constexpr static ImVec2 s_blackSize = {10.f, 85.f};
    inline static bool isBlackKey(uint8_t index)
    {
        const static std::unordered_set<uint8_t> blackKeysId = {1, 4, 6, 9, 11};
        return blackKeysId.contains(index % 12);
    }
};