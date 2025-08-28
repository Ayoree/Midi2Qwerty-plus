#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "common/Settings.h"

class SettingsWindow : private Drawable
{
private:
    static constexpr inline int WINDOW_FLAGS = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

private:
    // Singleton
    SettingsWindow();
    ~SettingsWindow() = default;

public:
    SettingsWindow(const SettingsWindow&) = delete;
    SettingsWindow& operator=(const SettingsWindow&) = delete;
    SettingsWindow(SettingsWindow&&) = delete;
    SettingsWindow& operator=(SettingsWindow&&) = delete;

    static SettingsWindow& instance()
    {
        static SettingsWindow instance = SettingsWindow();
        return instance;
    };
    
    void draw() override final;
};