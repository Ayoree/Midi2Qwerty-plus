#include "SettingsWindow.h"
#include "common/Logger.h"

SettingsWindow::SettingsWindow()
{
    LOG_DEBUG("SettingsWindow init");
}

void SettingsWindow::draw()
{
    ImGui::Begin("Settings");

    ImGui::Checkbox("Enable output", &Settings::instance().enableOutput);
    ImGui::Checkbox("Enable sustain", &Settings::instance().enableSustain);
    ImGui::Checkbox("Enable velocity", &Settings::instance().enableVelocity);

    ImGui::End();
}
