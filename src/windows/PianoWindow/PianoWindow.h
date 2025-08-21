#pragma once
#include "pch.h"
#include "Defines.h"
#include "common/Drawable.h"
#include "PianoKey.h"

class PianoWindow : private Drawable
{
public:
    // Singleton
    PianoWindow();
    ~PianoWindow() = default;
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
    
private:
    void setKeysPos();

private:
    constexpr static uint8_t KEYS_WHITE = 52;
    constexpr static uint8_t KEYS_BLACK = 36;
    constexpr static uint8_t KEYS_ALL = KEYS_WHITE + KEYS_BLACK;
    constexpr static int WINDOW_FLAGS = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove;
    
    std::array<std::unique_ptr<PianoKey>, KEYS_ALL> m_keys;
    std::array<PianoKey*, KEYS_WHITE> m_keysWhite;
    std::array<PianoKey*, KEYS_BLACK> m_keysBlack;
    
public:
    constexpr static Vec2<uint16_t> WINDOW_SIZE = {KEYS_WHITE * PianoKey::WHITE_KEY_SIZE.x, PianoKey::WHITE_KEY_SIZE.y};
};