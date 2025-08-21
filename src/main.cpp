#include "pch.h"
#include "Defines.h"
#include "common/Logger.h"
#include "windows/CustomTitleBar/CustomTitleBar.h"
#include "windows/SettingsWindow/SettingsWindow.h"
#include "windows/PianoWindow/PianoWindow.h"
#include "windows/LogWindow/LogWindow.h"

constexpr Vec2<uint16_t> WINDOW_SIZE = {PianoWindow::s_windowSize.x, 604};

static void glfw_error_callback(int error, const char* description)
{
    LOG_ERROR(std::format("GLFW Error {}: {}", error, description));
}

// Main code
int main(int, char**)
{
    Logger::instance(); // Init logger

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // Create window with graphics context
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow((int)(WINDOW_SIZE.x * main_scale) + 2, (int)(WINDOW_SIZE.y * main_scale), "MIDI 2 QWERTY plus", nullptr, nullptr);
    if (window == nullptr)
    {
        LOG_ERROR("Unable to initialize window!");
        return 1;
    }

    // Centralize window
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        int monitorX, monitorY, monitorWidth, monitorHeight;
        glfwGetMonitorWorkarea(monitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);
        const int x = monitorX + (monitorWidth - WINDOW_SIZE.x) * 0.5f;
        const int y = monitorY + (monitorHeight - WINDOW_SIZE.y) * 0.5f;
        glfwSetWindowPos(window, x, y);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = NULL; // Manually setting layout
    ImGui::LoadIniSettingsFromDisk("main_layout.ini");

    // Init custom title bar
    CustomTitleBar::instance().init(window);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Load Fonts
    io.Fonts->AddFontDefault();
    ImVec4 bgColor = COLOR_TRANSPARENT;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Draw custom windows
        CustomTitleBar::instance().draw();
        LogWindow::instance().draw();
        SettingsWindow::instance().draw();
        PianoWindow::instance().draw();

        //ImGui::ShowDemoWindow(&show_demo_window);
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
