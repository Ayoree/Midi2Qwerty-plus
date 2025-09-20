#pragma once

#include <functional>
#include <map>
#include "portmidi.h"
#include "porttime.h"
#include "Defines.h"
#include "AsyncMidiPoll.h"
#include "MidiPlayer.h"

class Midi
{
public:
    using PmDeviceMap = std::map<PmDeviceID, const PmDeviceInfo*>;

private:
    // Singleton
    Midi();
    ~Midi();

public:
    Midi(const Midi&) = delete;
    Midi& operator=(const Midi&) = delete;
    Midi(Midi&&) = delete;
    Midi& operator=(Midi&&) = delete;

    static Midi& instance()
    {
        static Midi instance = Midi();
        return instance;
    }

    inline const PmDeviceMap& getDevices() const { return m_devices; }
    const PmDeviceInfo* getSelectedInputDevice() const;
    const PmDeviceInfo* getSelectedOutputDevice() const;
    void setInputDevice(PmDeviceID newInputID);
    void setOutputDevice(PmDeviceID newOutputID);
    void openMidiFile(const std::string& filename);
    void closeMidiFile();
    void playMidiFile() const;
    void stopMidiFile();

private:
    bool handlePossibleError(const PmError err) const;
    void updateDevices();
    void setInputState(bool isEnabled = true);
    void setOutputState(bool isEnabled = true);

private:
    PmDeviceMap m_devices;
    MidiStream m_inputStream;
    MidiStream m_outputStream;
    std::uptr<AsyncMidiPoll> m_inputPoll = nullptr;
    std::uptr<AsyncMidiPoll> m_outputPoll = nullptr;
    std::uptr<MidiPlayer> m_midiPlayer = nullptr;
};