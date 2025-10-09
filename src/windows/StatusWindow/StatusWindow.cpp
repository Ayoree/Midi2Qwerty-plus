#include "StatusWindow.h"
#include "common/Settings.h"

void StatusWindow::draw()
{
    ImGui::Begin("Status", nullptr, WINDOW_FLAGS);
    if (!m_pressed.empty())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::TextColored(COLOR_WHITE, "Playing:\t");
        for (const auto& key : m_pressed)
        {
            ImGui::SameLine();
            ImGui::TextColored(COLOR_YELLOW, std::format("{:^5}", key->getName()).c_str());
        }
        ImGui::TextColored(COLOR_WHITE, "Qwerty:\t\t");
        for (const auto& key : m_pressed)
        {
            ImGui::SameLine();
            ImGui::TextColored(COLOR_YELLOW, std::format("{:^5}", key->getChar()).c_str());
        }
        if (Settings::instance().enableVelocity)
        {
            ImGui::TextColored(COLOR_WHITE, "Velocity:\t");
            ImGui::SameLine();
            ImGui::TextColored(COLOR_WHITE, std::format("{:.2f}", (double)PianoKey::getVelocity() / 127.f).c_str());
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void StatusWindow::addKey(PianoKey* key)
{
    m_pressed.insert(key);
}

void StatusWindow::removeKey(PianoKey* key)
{
    m_pressed.erase(key);
}
