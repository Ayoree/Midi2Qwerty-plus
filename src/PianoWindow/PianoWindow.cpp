#include "PianoWindow.h"
#include <thread>

PianoWindow::PianoWindow()
{
    uint8_t iWhite = 0;
    uint8_t iBlack = 0;
    for (uint8_t i = 0; i < KEYS_ALL; ++i)
    {
        m_keys[i] = std::make_unique<PianoKey>(i);
        if (m_keys[i]->isBlack())
            m_keysBlack[iBlack++] = m_keys[i].get();
        else
            m_keysWhite[iWhite++] = m_keys[i].get();
    }
    setKeysPos();
}
PianoWindow::~PianoWindow() {}

void PianoWindow::setKeysPos()
{
    uint8_t whiteIndex = 0;
    for (auto& key : m_keys)
    {
        if (key->isBlack())
        {
            key->setPos({
                    whiteIndex * PianoKey::s_whiteSize.x - PianoKey::s_blackSize.x / 2.f,
                    0
                });
        }
        else
        {
            key->setPos({
                    whiteIndex * PianoKey::s_whiteSize.x,
                    0
                });
            ++whiteIndex;
        }
    }
}

void PianoWindow::draw()
{
    ImGui::SetNextWindowSize(ImVec2(PianoKey::s_whiteSize.x * KEYS_WHITE, PianoKey::s_whiteSize.y + ImGui::GetFrameHeight()), ImGuiCond_Always);
    ImGui::Begin("Keyboard", NULL, s_windowFlags);

    for (const auto& key : m_keysWhite)
        key->draw();
    for (const auto& key : m_keysBlack)
        key->draw();

    ImGui::End();
}
