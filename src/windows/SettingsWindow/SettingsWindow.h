#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "common/Settings.h"

class SettingsWindow : private Drawable
{
public:
    // Singleton
    SettingsWindow();
    ~SettingsWindow() = default;
    SettingsWindow(const SettingsWindow&) = delete;
    SettingsWindow& operator=(const SettingsWindow&) = delete;
    SettingsWindow(SettingsWindow&&) = delete;
    SettingsWindow& operator=(SettingsWindow&&) = delete;
    static SettingsWindow& instance() // Singleton
    {
        static SettingsWindow instance = SettingsWindow();
        return instance;
    };
    void draw() override;

private:
    constexpr static int WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
};