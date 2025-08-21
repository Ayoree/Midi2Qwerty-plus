#include "PianoKey.h"
#include "pch.h"
#include "Defines.h"
#include "common/Logger.h"

PianoKey::PianoKey(uint8_t keyId) :
    m_id(keyId),
    m_isBlack(PianoKey::isBlackKey(keyId))
{
    //LOG_DEBUG(std::format("Created `PianoKey`, id: {}, isBlack: {}", m_id, m_isBlack));
}
    
void PianoKey::draw()
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    const ImVec2 targetPos = ImGui::GetWindowPos() + m_pos + ImVec2(0, ImGui::GetFrameHeight());

    ImColor color = m_isPressed ? COLOR_YELLOW : m_isBlack ? COLOR_BLACK : COLOR_WHITE;

    const ImVec2 size = {
        m_isBlack ? BLACK_KEY_SIZE.x : WHITE_KEY_SIZE.x,
        m_isBlack ? BLACK_KEY_SIZE.y : WHITE_KEY_SIZE.y
    };
    draw_list->AddRectFilled(
        targetPos,
        ImVec2(targetPos.x + size.x, targetPos.y + size.y),
        color, m_isBlack ? BLACK_KEY_SIZE.x / 5.f : WHITE_KEY_SIZE.x / 7.5f, ImDrawFlags_RoundCornersBottom
    );
    if (!m_isBlack)
    {
        draw_list->AddRect(
            targetPos,
            ImVec2(targetPos.x + WHITE_KEY_SIZE.x, targetPos.y + WHITE_KEY_SIZE.y),
            COLOR_BLACK,  WHITE_KEY_SIZE.x / 7.5f, ImDrawFlags_RoundCornersBottom, 0.5f
        ); 
    }
}
