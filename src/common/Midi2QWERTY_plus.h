#pragma once

#include "pch.h"
#include "Defines.h"
#include "common/Logger.h"
#include "common/midi/Midi.h"
#include "windows/CustomTitleBar/CustomTitleBar.h"
#include "windows/LogWindow/LogWindow.h"
#include "windows/StatusWindow/StatusWindow.h"
#include "windows/SettingsWindow/SettingsWindow.h"
#include "windows/PianoWindow/PianoWindow.h"

constexpr Vec2<uint16_t> WINDOW_SIZE = {PianoWindow::WINDOW_SIZE.x, 604};

class Midi2QWERTY_plus
{
public:
    Midi2QWERTY_plus();
    ~Midi2QWERTY_plus();

    bool init();
    bool run();
private:
    GLFWwindow* m_window = nullptr;
    ImVec4 m_bgColor = COLOR_TRANSPARENT;
};