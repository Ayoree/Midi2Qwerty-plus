#include "LogWindow.h"
#include "common/Logger.h"

void LogWindow::draw()
{
    ImGui::Begin("Log", NULL, s_windowFlags);
    ImGui::BeginChild("Scroll", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGuiListClipper clipper;
    clipper.Begin(Logger::instance().getBuffer().size());
    for (const auto& entry : Logger::instance().getBuffer())
    {
        ImGui::TextColored(COLOR_GRAY, std::format("[{}]", entry.get_time()).c_str());
        ImGui::SameLine();
        ImGui::TextColored(LogEntry::to_color(entry.type), std::format("[{}] ", LogEntry::to_string(entry.type)).c_str());
        ImGui::SameLine();
        ImGui::TextColored(LogEntry::to_color(entry.type), entry.msg.c_str());
    }
    clipper.End();
    
    ImGui::PopStyleVar();

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}
