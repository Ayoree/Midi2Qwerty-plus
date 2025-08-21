#include "PianoKey.h"
#include "pch.h"
#include "Defines.h"
#include "common/Logger.h"

PianoKey::PianoKey(uint8_t keyId) :
    m_id(keyId),
    m_isBlack(PianoKey::isBlackKey(keyId))
{
    LOG_DEBUG(std::format("Created `PianoKey`, id: {}, isBlack: {}", m_id, m_isBlack));
}

PianoKey::~PianoKey() {}
    
void PianoKey::draw()
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    const ImVec2 targetPos = ImGui::GetWindowPos() + m_pos + ImVec2(0, ImGui::GetFrameHeight());
    const ImVec2 size = {
        m_isBlack ? s_blackSize.x : s_whiteSize.x,
        m_isBlack ? s_blackSize.y : s_whiteSize.y
    };
    draw_list->AddRectFilled(
        targetPos,
        ImVec2(targetPos.x + size.x, targetPos.y + size.y),
        m_isBlack ? COLOR_BLACK : COLOR_WHITE, m_isBlack ? s_blackSize.x / 5.f : s_whiteSize.x / 7.5f, ImDrawFlags_RoundCornersBottom
    );
    if (!m_isBlack)
    {
        draw_list->AddRect(
            targetPos,
            ImVec2(targetPos.x + s_whiteSize.x, targetPos.y + s_whiteSize.y),
            COLOR_BLACK,  s_whiteSize.x / 7.5f, ImDrawFlags_RoundCornersBottom, 0.5f
        ); 
    }
}
