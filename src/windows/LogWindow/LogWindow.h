#pragma once
#include <fstream>
#include "pch.h"
#include "common/Drawable.h"

class LogWindow : private Drawable
{
public:
    LogWindow();

    static LogWindow& instance() // Singleton
    {
        static LogWindow instance = LogWindow();
        return instance;
    };
    void draw() override;

private:
    constexpr static int s_windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
};