cmake_minimum_required(VERSION 3.20)
project(Midi2Qwerty_plus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

# Add GLFW as submodule
add_subdirectory(submodules/glfw)
# Disable GLFW uneccecary options 
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
find_package(OpenGL REQUIRED)

add_compile_options(
    -Wall
    -Werror
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Add ImGui as a library
add_library(imgui
    submodules/imgui/imgui.cpp
    submodules/imgui/imgui_draw.cpp
    submodules/imgui/imgui_tables.cpp
    submodules/imgui/imgui_widgets.cpp
    submodules/imgui/misc/cpp/imgui_stdlib.cpp
    submodules/imgui/backends/imgui_impl_glfw.cpp
    submodules/imgui/backends/imgui_impl_opengl3.cpp
    submodules/imgui/imgui_demo.cpp # TODO: Remove later
)

# Define the executable FIRST
add_executable(Midi2Qwerty_plus src/main.cpp)

# Add include libraries
target_include_directories(imgui PUBLIC
    submodules/imgui
    submodules/imgui/backends
    submodules/imgui/misc/cpp
    submodules/glfw/include
)

# THEN link libraries to it
target_link_libraries(Midi2Qwerty_plus PRIVATE
    imgui
    glfw
    OpenGL::GL
)

# Add compile definitions
target_compile_definitions(Midi2Qwerty_plus PUBLIC
    UNICODE
    _UNICODE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:_RELEASE>
)
