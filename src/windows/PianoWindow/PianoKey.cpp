#include "PianoKey.h"
#include "common/Logger.h"
#include "windows/StatusWindow/StatusWindow.h"
#include "common/Settings.h"

PianoKey::PianoKey(uint8_t keyId) :
    m_id(keyId),
    m_isBlack(generateIsBlack(this)),
    m_name(generateName(this)),
    m_qwertyChar(generateQwertyChar(this)),
    m_needShift(generateIsShift(this)),
    m_needCtrl(generateIsCtrl(this)),
    m_scanCode(generateScanCode(this))
{
    
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

void PianoKey::press()
{
    m_isPressed = true;
    StatusWindow::instance().addKey(this);

    if (Settings::instance().enableOutput)
    {
        std::vector<INPUT> inputVec;
        inputVec.reserve(6);
        // Releasing key if already pressed
        if (KEY_PRESSED(GetAsyncKeyState(MapVirtualKey(m_scanCode, MAPVK_VSC_TO_VK))))
            inputVec.push_back(createInput(m_scanCode, false));

        // Press ctrl
        if (m_needCtrl)
            inputVec.push_back(createInput(MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC), true));
        // Press shift
        if (m_needShift)
            inputVec.push_back(createInput(MapVirtualKey(VK_SHIFT, MAPVK_VK_TO_VSC), true));

        // Press key
        inputVec.push_back(createInput(m_scanCode, true));

        // Release ctrl
        if (m_needCtrl)
            inputVec.push_back(createInput(MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC), false));
        // Release shift
        if (m_needShift)
            inputVec.push_back(createInput(MapVirtualKey(VK_SHIFT, MAPVK_VK_TO_VSC), false));

        SendInput(inputVec.size(), inputVec.data(), sizeof(INPUT));
    }
}

void PianoKey::release()
{
    m_isPressed = false;
    StatusWindow::instance().removeKey(this);
    if (Settings::instance().enableOutput)
    {
        INPUT input = createInput(m_scanCode, false);
        SendInput(1, &input, sizeof(INPUT));
    }
}

const std::unordered_map<char, char>& PianoKey::getSpecialChars()
{
    static const std::unordered_map<char, char> specialChars = {
        {'!', '1'},
        {'@', '2'},
        {'#', '3'},
        {'$', '4'},
        {'%', '5'},
        {'^', '6'},
        {'&', '7'},
        {'*', '8'},
        {'(', '9'},
        {')', '0'}
    };
    return specialChars;
};

const uint8_t PianoKey::midiCode2KeyIndex(const uint32_t code)
{
    return code - FIRST_KEY_OFFSET;
}

const void PianoKey::pressSpace()
{
    if (Settings::instance().enableOutput)
    {
        INPUT input = createInput(MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC), true);
        SendInput(1, &input, sizeof(INPUT));
    }
}

const void PianoKey::releaseSpace()
{
    if (Settings::instance().enableOutput)
    {
        INPUT input = createInput(MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC), false);
        SendInput(1, &input, sizeof(INPUT));
    }
}

const bool PianoKey::generateIsBlack(PianoKey* key)
{
    const static std::unordered_set<uint8_t> blackKeysId = {1, 4, 6, 9, 11};
    return blackKeysId.contains(key->getIndex() % 12);
}

const std::string PianoKey::generateName(PianoKey* key)
{
    const static std::array<std::string_view, 12> keyNames = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    std::stringstream ss;
    ss << keyNames.at(key->getIndex() % keyNames.size());
    ss << (key->getIndex() + 9) / keyNames.size();
    return ss.str();
}

const char PianoKey::generateQwertyChar(PianoKey* key)
{
    IM_ASSERT(key->getIndex() < QWERTY_LIST.size());
    return QWERTY_LIST.at(key->getIndex());
}

const WORD PianoKey::generateScanCode(PianoKey* key)
{
    IM_ASSERT(key->getIndex() < QWERTY_LIST.size());
    char ch = QWERTY_LIST.at(key->getIndex());
    if (ch >= 'a' && ch <= 'z')
        return MapVirtualKey('A' + (ch - 'a'), MAPVK_VK_TO_VSC);
    if (getSpecialChars().contains(ch))
        return MapVirtualKey(getSpecialChars().at(ch), MAPVK_VK_TO_VSC);
    return MapVirtualKey(QWERTY_LIST.at(key->getIndex()), MAPVK_VK_TO_VSC);
}

const  bool PianoKey::generateIsShift(PianoKey* key)
{
    IM_ASSERT(key->getIndex() < QWERTY_LIST.size());
    char ch = QWERTY_LIST.at(key->getIndex());
    return (ch >= 'A' && ch <= 'Z') || getSpecialChars().contains(ch);
}

const bool PianoKey::generateIsCtrl(PianoKey* key)
{
    return key->getIndex() < COMMON_NOTES_OFFSET || key->getIndex() > (COMMON_NOTES_OFFSET + COMMON_NOTES_AMOUNT);
}

const INPUT PianoKey::createInput(WORD scanCode, bool isPress)
{
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = scanCode;
    input.ki.dwFlags = KEYEVENTF_SCANCODE;
    if (!isPress)
        input.ki.dwFlags |= KEYEVENTF_KEYUP;

    return input;
}
