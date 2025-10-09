#include "Midi2QWERTY_plus.h"

static void glfw_error_callback(int error, const char* description)
{
    LOG_ERROR(std::format("GLFW Error {}: {}", error, description));
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_F5)
        {
            if (mods & GLFW_MOD_SHIFT)
            {
                LOG_INFO("Stop playing MIDI-file");
                Midi::instance().stopMidiFile();
            }
            else
            {
                LOG_INFO("Start playing MIDI-file");
                Midi::instance().playMidiFile();
            }
        }
    }
}

Midi2QWERTY_plus::Midi2QWERTY_plus(){}

Midi2QWERTY_plus::~Midi2QWERTY_plus()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Midi2QWERTY_plus::init()
{
    Logger::instance(); // Init logger
    Midi::instance(); // Init PortMidi

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        LOG_ERROR("Unable to initialize GLFW");
        return false;
    }

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
    m_window = glfwCreateWindow((int)(WINDOW_SIZE.x * main_scale) + 2, (int)(WINDOW_SIZE.y * main_scale), "MIDI 2 QWERTY plus", nullptr, nullptr);
    if (!m_window)
    {
        LOG_ERROR("Unable to initialize window");
        return false;
    }

    // Centralize window
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        int monitorX, monitorY, monitorWidth, monitorHeight;
        glfwGetMonitorWorkarea(monitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);
        const int x = monitorX + (monitorWidth - WINDOW_SIZE.x) * 0.5f;
        const int y = monitorY + (monitorHeight - WINDOW_SIZE.y) * 0.5f;
        glfwSetWindowPos(m_window, x, y);
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_FALSE);
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
    CustomTitleBar::instance().init(m_window);

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
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Load Fonts
    io.Fonts->AddFontDefault();
    
    glfwSetKeyCallback(m_window, keyCallback); // Keys callback
    // Creating songs folder
    if (!std::filesystem::exists("songs"))
        if (!std::filesystem::create_directory("songs"))
            LOG_ERROR("Failed to create songs directory");
    
    return true;
}

bool Midi2QWERTY_plus::run()
{
    if (glfwWindowShouldClose(m_window))
        return false;

    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw custom windows
    CustomTitleBar::instance().draw();
    LogWindow::instance().draw();
    StatusWindow::instance().draw();
    SettingsWindow::instance().draw();
    PianoWindow::instance().draw();

    //ImGui::ShowDemoWindow();
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_bgColor.x, m_bgColor.y, m_bgColor.z, m_bgColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);

    return true;
}
