#pragma once
#include "pch.h"

struct Settings
{
public:
    // Singleton
    Settings() {};
    ~Settings() {};
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;
    static Settings& instance()
    {
        static Settings instance = Settings();
        return instance;
    }

    bool enableOutput = false;
    bool enableSustain = true;
    bool enableVelocity = false;
};
