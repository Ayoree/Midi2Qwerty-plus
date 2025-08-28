#pragma once
#include "pch.h"

struct Settings
{
private:
    // Singleton
    Settings() = default;
    ~Settings() = default;

public:
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;

    static Settings& instance()
    {
        static Settings instance = Settings();
        return instance;
    }

public:
    bool enableOutput = false;
    bool enableSustain = true;
    bool enableVelocity = false;
};
