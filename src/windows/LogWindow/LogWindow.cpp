#include "LogWindow.h"

void LogWindow::draw()
{
    ImGui::Begin("Log", nullptr, WINDOW_FLAGS);
    ImGui::BeginChild("Scroll", ImVec2(0,0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGuiListClipper clipper;
    clipper.Begin(Logger::instance().getBuffer().size());
    for (const auto& entry : Logger::instance().getBuffer())
    {
        ImGui::TextColored(COLOR_GRAY, std::format("[{}]", entry.getTimeStr()).c_str());
        ImGui::SameLine();
        ImGui::TextColored(LogEntry::toColor(entry.type), std::format("[{}] ", LogEntry::typeToString(entry.type)).c_str());
        ImGui::SameLine();
        ImGui::TextColored(LogEntry::toColor(entry.type), entry.msg.c_str());
    }
    clipper.End();
    
    ImGui::PopStyleVar();

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}
