#include "pch.h"
#include "Defines.h"
#include "common/Midi2QWERTY_plus.h"

// Main code
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    std::ofstream logFile(LOG_FILE_PATH.data());
    std::cout.rdbuf(logFile.rdbuf());
    std::cerr.rdbuf(logFile.rdbuf());
    std::clog.rdbuf(logFile.rdbuf());

    Midi2QWERTY_plus midi2qwerty;
    if (!midi2qwerty.init())
        return -1;

    while (midi2qwerty.run());

    fclose(stdout);
    fclose(stderr);
    
    return 0;
}
