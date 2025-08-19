#include "LogWindow.h"

LogWindow::LogWindow()
{
    m_logfile.open("log.txt", std::ios::out | std::ios::trunc);
    if (!m_logfile.is_open()) {
        LOG_ERROR("Failed to open `log.txt`");
    }
}

LogWindow::~LogWindow()
{
    if (m_logfile.is_open()) {
        m_logfile.close();
    }
}

void LogWindow::draw()
{
    ImGui::Begin("Log", NULL, s_windowFlags);
    ImGui::BeginChild("Scroll", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGuiListClipper clipper;
    clipper.Begin(m_buf.size());
    for (const auto& entry : m_buf)
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
