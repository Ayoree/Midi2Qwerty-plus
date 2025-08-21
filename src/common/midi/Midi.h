#pragma once

#include <functional>
#include <map>
#include "portmidi.h"
#include "porttime.h"
#include "Defines.h"
#include "AsyncMidiPoll.h"

typedef std::map<size_t, const PmDeviceInfo*> PmDeviceMap;

class Midi {
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

    void updateDevices();
    const PmDeviceMap& getDevices() const { return m_devices; }
    const PmDeviceInfo* getSelectedInputDevice() const;
    const PmDeviceInfo* getSelectedOutputDevice() const;
    void setInputDevice(PmDeviceID newInputID);
    void setOutputDevice(PmDeviceID newOutputID);
    void setInput(bool isEnabled = true);
    void setOutput(bool isEnabled = true);

private:
    bool handlePossibleError(const PmError err) const;

private:
    PmDeviceMap m_devices;
    MidiStream m_inputStream;
    MidiStream m_outputStream;
    std::uptr<AsyncMidiPoll> m_inputPoll;
    std::uptr<AsyncMidiPoll> m_outputPoll;
};