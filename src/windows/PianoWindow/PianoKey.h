#pragma once
#include "pch.h"
#include "Defines.h"
#include "common/Drawable.h"

#define KEY_PRESSED(state) ((state) & 0b1000000000000000)
#define KEY_RELEASED(state) ((state) & 0b0000000000000001)

class PianoKey : private Drawable
{
public:
    static constexpr inline ImVec2 WHITE_KEY_SIZE = {18.f, 120.f};
    static constexpr inline ImVec2 BLACK_KEY_SIZE = {10.f, 85.f};
    static constexpr inline uint8_t KEYS_WHITE = 52;
    static constexpr inline uint8_t KEYS_BLACK = 36;
    static constexpr inline uint8_t KEYS_ALL = KEYS_WHITE + KEYS_BLACK;

private:
    // Used for converting from MidiCode to key index
    static constexpr inline uint8_t FIRST_KEY_OFFSET = 0x15;
    // Piano keyboard keys, that should be pressed
    static constexpr inline std::array<char, KEYS_ALL + 1> QWERTY_LIST = {"1234567890qwert1!2@34$5%6^78*9(0qQwWeErtTyYuiIoOpPasSdDfgGhHjJklLzZxcCvVbBnmyuiopasdfghj"};
    // Decides amount of keys (left side keys), that should be pressed WITH CTRL
    static constexpr inline uint8_t COMMON_NOTES_OFFSET = 15;
    // Decides amount of keys, that should be pressed WITHOUT CTRL
    static constexpr inline uint8_t COMMON_NOTES_AMOUNT = 60;
    // Velocities
    static constexpr inline std::array<char, 33> VELOCITIES = {"1234567890qwertyuiopasdfghjklzxc"};
    static unsigned short CURRENT_VELOCITY;
    
public:
    static const uint8_t midiCode2KeyIndex(const uint32_t code);
    static const void pressSpace();
    static const void releaseSpace();
    static const void setVelocity(int velocity);
    static const unsigned short getVelocity() { return CURRENT_VELOCITY; }
    
private:
    // Returns chars, that require pressing shift to type them
    static inline const std::unordered_map<char, char>& getSpecialChars();
    static inline const bool generateIsBlack(PianoKey* key);
    static inline const std::string generateName(PianoKey* key);
    static inline const char generateQwertyChar(PianoKey* key);
    static inline const WORD generateScanCode(PianoKey* key);
    static inline const bool generateIsShift(PianoKey* key);
    static inline const bool generateIsCtrl(PianoKey* key);
    static inline const INPUT createInput(WORD vkKeyCode, bool isPress);

public:
    explicit PianoKey(uint8_t keyId);
    ~PianoKey() = default;

    void draw() override final;

    inline bool isBlack() const { return m_isBlack; }
    inline uint8_t getIndex() const { return m_id; }
    inline float getWidth() const { return m_isBlack ? BLACK_KEY_SIZE.x : WHITE_KEY_SIZE.x; }
    inline std::string_view getName() const { return m_name; }
    inline char getChar() const { return m_qwertyChar; }
    inline void setPos(ImVec2 pos) { m_pos = pos; }

    bool isPressed() const { return m_isPressed; }
    void press();
    void release();
    
private:
    const uint8_t m_id;
    const bool m_isBlack;
    const std::string m_name;
    const char m_qwertyChar;
    const bool m_needShift;
    const bool m_needCtrl;
    const WORD m_scanCode;
    ImVec2 m_pos = {0, 0};
    bool m_isPressed = false;
};