#include "CustomTitleBar.h"

void CustomTitleBar::init(GLFWwindow* window)
{
    IM_ASSERT(window != nullptr);
    m_window = window;
    LOG_DEBUG("CustomTitleBar inited");
}

void CustomTitleBar::draw()
{
    ImGui::Begin("TitleBar", nullptr, WINDOW_FLAGS);
    ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, s_windowHeight));
    ImGui::SetWindowPos(ImVec2(0, 0));

    handleDrag();

    // Hide button
    ImGui::SameLine(ImGui::GetWindowWidth() - 78);
    if (ImGui::Button("_", ImVec2(30, 30)))
    {
        glfwIconifyWindow(m_window);
    }
    // Close button
    ImGui::SameLine();
    if (ImGui::Button("X", ImVec2(30, 30)))
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f - 50);
    ImGui::Text(APP_NAME.data());
    
    ImGui::End();
}

void CustomTitleBar::handleDrag()
{
    static bool isDragging = false;
    static bool isMarginReached = false;
    static Vec2f startMousePos;
    static Vec2i startWndPos;
    static Vec2i newWndPos;
    
    static bool isErrorShowed = false;
    if (m_window == nullptr && !isErrorShowed) [[unlikely]]
    {
        isErrorShowed = true;
        LOG_ERROR("CustomTileBar: `m_window` is nullptr");
        return;
    }

    const ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsWindowHovered() && !isDragging && io.MouseDownDuration[MOUSE_LEFT] == 0.f) {
        isDragging = true;
        isMarginReached = false;
        startMousePos = Vec2f(io.MousePos);
        glfwGetWindowPos(m_window, &startWndPos.x, &startWndPos.y);
        newWndPos = startWndPos;
    }
    
    if (isDragging) {
        Vec2f curPos = Vec2f(io.MousePos);
        Vec2f delta = curPos - startMousePos;
        if (isMarginReached || abs(delta.x) > DRAG_MARGIN || abs(delta.y) > DRAG_MARGIN)
        {
            isMarginReached = true;
            newWndPos += Vec2i(floorf(delta.x), floorf(delta.y));;
            glfwSetWindowPos(m_window, newWndPos.x, newWndPos.y);
        }
        
        if (!io.MouseDown[MOUSE_LEFT])
            isDragging = false;
    }
}
