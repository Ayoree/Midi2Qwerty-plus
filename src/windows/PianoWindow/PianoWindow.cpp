#include "PianoWindow.h"
#include "common/Logger.h"
PianoWindow::PianoWindow()
{
    uint8_t iWhite = 0;
    uint8_t iBlack = 0;
    for (uint8_t i = 0; i < m_keys.size(); ++i)
    {
        m_keys[i] = make::uptr<PianoKey>(i);
        if (m_keys[i]->isBlack())
            m_keysBlack[iBlack++] = m_keys[i].get();
        else
            m_keysWhite[iWhite++] = m_keys[i].get();
    }
    setKeysPos();
    LOG_DEBUG("PianoWindow inited");
}

void PianoWindow::setKeysPos()
{
    uint8_t whiteIndex = 0;
    for (auto& key : m_keys)
    {
        if (key->isBlack())
        {
            key->setPos({
                    whiteIndex * PianoKey::WHITE_KEY_SIZE.x - PianoKey::BLACK_KEY_SIZE.x * 0.5f + 1.f,
                    0
                });
        }
        else
        {
            key->setPos({
                    whiteIndex * PianoKey::WHITE_KEY_SIZE.x + 1.f,
                    0
                });
            ++whiteIndex;
        }
    }
}

void PianoWindow::draw()
{
    ImGui::SetNextWindowSize(ImVec2(WINDOW_SIZE.x + 2, WINDOW_SIZE.y + ImGui::GetFrameHeight() + 1.f), ImGuiCond_Always);
    ImGui::Begin("Keyboard", nullptr, WINDOW_FLAGS);

    for (const auto& key : m_keysWhite)
        key->draw();
    for (const auto& key : m_keysBlack)
        key->draw();

    ImGui::End();
}

void PianoWindow::pressKey(uint8_t keyID) const
{
    if (m_keys.size() <= keyID) [[unlikely]] {
        LOG_ERROR(std::format("PianoWindow::pressKey: keyID: `{}` is out of bounds of available keys", keyID));
        return;
    }
    m_keys.at(keyID)->press();
}

void PianoWindow::releaseKey(uint8_t keyID) const
{
    if (m_keys.size() <= keyID) [[unlikely]] {
        LOG_ERROR(std::format("PianoWindow::releaseKey keyID: `{}` is out of bounds of available keys", keyID));
        return;
    }
    m_keys.at(keyID)->release();
}
