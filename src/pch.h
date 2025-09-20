#pragma once
#define NOGDI // remove GDI from windows.h
#include <windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>
#include <cmath>
#include <unordered_set>
#include <set>
#include <map>
#include <algorithm>
#include <execution>
#include <memory>
#include <array>
#include <deque>
#include <stop_token>
#include <thread>
#include <mutex>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"