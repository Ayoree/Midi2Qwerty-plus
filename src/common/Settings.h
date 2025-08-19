#pragma once
#include "pch.h"

struct Settings
{
    static Settings& instance() // Singleton
    {
        static Settings instance = Settings();
        return instance;
    }

    bool enableOutput = false;
    bool enableSustain = true;
    bool enableVelocity = false;
};
