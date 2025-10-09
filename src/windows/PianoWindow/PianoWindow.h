#pragma once
#include "pch.h"
#include "Defines.h"
#include "common/Drawable.h"
#include "PianoKey.h"

class PianoWindow : private Drawable
{
private:
    static constexpr inline int WINDOW_FLAGS = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove;
public:
    static constexpr inline Vec2<uint16_t> WINDOW_SIZE = {PianoKey::KEYS_WHITE * PianoKey::WHITE_KEY_SIZE.x, PianoKey::WHITE_KEY_SIZE.y};

private:
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

    void draw() override final;
    void pressKey(uint8_t keyID) const;
    void releaseKey(uint8_t keyID) const;
    void releaseAll() const;
    
private:
    void setKeysPos();

private:
    std::array<std::uptr<PianoKey>, PianoKey::KEYS_ALL> m_keys;
    std::array<PianoKey*, PianoKey::KEYS_WHITE> m_keysWhite;
    std::array<PianoKey*, PianoKey::KEYS_BLACK> m_keysBlack;
};