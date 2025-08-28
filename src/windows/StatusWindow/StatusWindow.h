#pragma once
#include "pch.h"
#include "common/Drawable.h"
#include "common/Logger.h"
#include "windows/PianoWindow/PianoKey.h"

class StatusWindow : private Drawable
{
private:
    static constexpr inline int WINDOW_FLAGS = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

private:
    // Singleton
    StatusWindow() { LOG_DEBUG("StatusWindow inited"); };
    ~StatusWindow() = default;

public:
    StatusWindow(const StatusWindow&) = delete;
    StatusWindow& operator=(const StatusWindow&) = delete;
    StatusWindow(StatusWindow&&) = delete;
    StatusWindow& operator=(StatusWindow&&) = delete;

    static StatusWindow& instance()
    {
        static StatusWindow instance = StatusWindow();
        return instance;
    };

    void draw() override final;
    void addKey(PianoKey* key);
    void removeKey(PianoKey* key);

private:
    struct SortByIndex {
        bool operator()(const PianoKey* a, const PianoKey* b) const { return a->getIndex() < b->getIndex(); }
    };

    std::set<PianoKey*, SortByIndex> m_pressed;
};