#pragma once
#include "Defines.h"
#include "pch.h"
#include "MidiFile.h"
#include "portmidi.h"
#include "MidiFileEvent.h"

using namespace smf;

class MidiPlayer
{
public:
    MidiPlayer(const std::deque<MidiFileEvent> events);
    ~MidiPlayer();
    void setOutputStream(PortMidiStream* stream);
    bool isRunning() const { return m_isRunning.load(); }
    void start();
    void stop();

private:
    void startPoll(std::stop_token stoken);

private:
    std::atomic<PortMidiStream*> m_outputStream;
    std::deque<MidiFileEvent> m_events;
    std::uptr<std::jthread> m_workerThread = nullptr;
    std::mutex m_mtx;
    std::atomic<bool> m_isRunning = false;
    double m_startTime;
    double m_lastEventTime = 0.0;
};