#pragma once
#include "pch.h"
#include "Defines.h"
#include "common/Drawable.h"
#include "PianoKey.h"
#include "common/utils/PianoUtils.h"

class PianoWindow : private Drawable
{
    // Singleton
    PianoWindow();
    ~PianoWindow() = default;
public:
    PianoWindow(const PianoWindow&) = delete;
    PianoWindow& operator=(const PianoWindow&) = delete;
    PianoWindow(PianoWindow&&) = delete;
    PianoWindow& operator=(PianoWindow&&) = delete;
    static PianoWindow& instance()
    {
        static PianoWindow instance = PianoWindow();
        return instance;
    };
    void draw() override;

    void pressKey(uint8_t keyID) const;
    void releaseKey(uint8_t keyID) const;
    
private:
    void setKeysPos();

private:
    
    constexpr static int WINDOW_FLAGS = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove;
    
    std::array<std::uptr<PianoKey>, PianoUtils::KEYS_ALL> m_keys;
    std::array<PianoKey*, PianoUtils::KEYS_WHITE> m_keysWhite;
    std::array<PianoKey*, PianoUtils::KEYS_BLACK> m_keysBlack;
    
public:
    constexpr static Vec2<uint16_t> WINDOW_SIZE = {PianoUtils::KEYS_WHITE * PianoKey::WHITE_KEY_SIZE.x, PianoKey::WHITE_KEY_SIZE.y};
};