#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "PianoKey.h"
#include <array>

#define WND_PIANO PianoWindow::instance()

class PianoWindow : private Drawable
{
public:
    PianoWindow();
    ~PianoWindow();
    
    static PianoWindow& instance() // Singleton
    {
        static PianoWindow instance = PianoWindow();
        return instance;
    };
    void draw() override;
    
private:
    void setKeysPos();

private:
    constexpr static int s_windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse;
    constexpr static uint8_t KEYS_WHITE = 52;
    constexpr static uint8_t KEYS_BLACK = 36;
    constexpr static uint8_t KEYS_ALL = KEYS_WHITE + KEYS_BLACK;

    std::array<std::unique_ptr<PianoKey>, KEYS_ALL> m_keys;
    std::array<PianoKey*, KEYS_WHITE> m_keysWhite;
    std::array<PianoKey*, KEYS_BLACK> m_keysBlack;
};