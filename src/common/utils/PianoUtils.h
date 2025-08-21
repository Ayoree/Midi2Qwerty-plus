#pragma once

#include <cstdint>

class PianoUtils
{
private:
    constexpr static uint8_t FIRST_KEY_OFFSET = 0x15;
public:
    constexpr static uint8_t KEYS_WHITE = 52;
    constexpr static uint8_t KEYS_BLACK = 36;
    constexpr static uint8_t KEYS_ALL = KEYS_WHITE + KEYS_BLACK;

    const static uint8_t code2Key(const uint32_t& code) { return code - FIRST_KEY_OFFSET; }
};