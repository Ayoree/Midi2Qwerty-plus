#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "common/Logger.h"

class CustomTitleBar : private Drawable
{
public:
    static CustomTitleBar& instance() // Singleton
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
    constexpr static int s_windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
    constexpr static float s_toDragMargin = 6.f;
};