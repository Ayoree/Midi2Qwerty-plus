#pragma once
#include <fstream>
#include "pch.h"
#include "common/Drawable.h"
#include "common/Logger.h"

class LogWindow : private Drawable
{
    // Singleton
    LogWindow() { LOG_DEBUG("LogWindow inited"); };
    ~LogWindow() = default;
public:
    LogWindow(const LogWindow&) = delete;
    LogWindow& operator=(const LogWindow&) = delete;
    LogWindow(LogWindow&&) = delete;
    LogWindow& operator=(LogWindow&&) = delete;

    static LogWindow& instance() // Singleton
    {
        static LogWindow instance = LogWindow();
        return instance;
    };
    void draw() override;

private:
    constexpr static int WINDOW_FLAGS = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
};