#pragma once

#include <thread>
#include <stop_token>
#include "portmidi.h"
#include "porttime.h"
#include "pch.h"
#include "common/Logger.h"

using MidiStreamCallback = std::function<void(PmTimestamp, uint8_t, PmMessage, PmMessage)>;

struct MidiStream
{
    static constexpr uint16_t BUF_SIZE = 1024;
    PmDeviceID deviceID = pmNoDevice;
    PortMidiStream* stream = nullptr;
    PmEvent buffer[BUF_SIZE];
};

class AsyncMidiPoll
{
private:
    static constexpr inline std::chrono::nanoseconds SLEEP_TIME = std::chrono::milliseconds(1);
    static constexpr inline std::chrono::nanoseconds LONG_SLEEP_TIME = std::chrono::milliseconds(100);
    static constexpr inline std::chrono::nanoseconds LONG_SLEEP_TIME_MARGIN = std::chrono::seconds(30);

public:
    AsyncMidiPoll(MidiStream& stream);
    ~AsyncMidiPoll();

    void setOutputStream(PortMidiStream* stream);

private:
    void startPoll(std::stop_token stoken);
    void readStreamData();

private:
    MidiStream& m_stream;
    std::atomic<PortMidiStream*> m_outputStream;
    std::jthread m_workerThread;
    std::chrono::steady_clock::time_point m_lastEventTime;
};