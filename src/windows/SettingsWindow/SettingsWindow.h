#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "common/Settings.h"

class SettingsWindow : private Drawable
{
public:
    SettingsWindow();
    
    static SettingsWindow& instance() // Singleton
    {
        static SettingsWindow instance = SettingsWindow();
        return instance;
    };
    void draw() override;
};