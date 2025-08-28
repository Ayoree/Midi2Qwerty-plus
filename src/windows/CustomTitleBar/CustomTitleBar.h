#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "common/Logger.h"

class CustomTitleBar : private Drawable
{
private:
    static constexpr inline int WINDOW_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
    static constexpr inline float DRAG_MARGIN = 6.f;
public:
    static constexpr inline int s_windowHeight = 46;

private:
    // Singleton
    CustomTitleBar() = default;
    ~CustomTitleBar() = default;

public:
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

    void draw() override final;
    void handleDrag();

private:
    GLFWwindow* m_window = nullptr;
};