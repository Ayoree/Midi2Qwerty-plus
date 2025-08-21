#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "common/Logger.h"

class CustomTitleBar : private Drawable
{
public:
    // Singleton
    CustomTitleBar() = default;
    ~CustomTitleBar() = default;
    CustomTitleBar(const CustomTitleBar&) = delete;
    CustomTitleBar& operator=(const CustomTitleBar&) = delete;
    CustomTitleBar(CustomTitleBar&&) = delete;
    CustomTitleBar& operator=(CustomTitleBar&&) = delete;
    static CustomTitleBar& instance()
    {
        static CustomTitleBar instance = CustomTitleBar();
        return instance;
    };
    void init(GLFWwindow* window);

    void draw() override;
    void handleDrag();

private:
    GLFWwindow* m_window = nullptr;

public:
    constexpr static int s_windowHeight = 46;

private:
    constexpr static int WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
    constexpr static float DRAG_MARGIN = 6.f;
};