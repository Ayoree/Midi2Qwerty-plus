#pragma once

#include <functional>
#include <map>
#include "portmidi.h"
#include "porttime.h"

typedef std::function<void(PmTimestamp, uint8_t, PmMessage, PmMessage)> MidiCallback;
typedef std::map<size_t, const PmDeviceInfo*> PmDeviceMap;

class Midi {
public:
    // Singleton
    Midi();
    ~Midi();
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
    PmDeviceMap m_devices;
    PmDeviceID m_selectedInputID;
    PmDeviceID m_selectedOutputID;
    PortMidiStream* m_inputStream = nullptr;
    PortMidiStream* m_outputStream = nullptr;

private:
    constexpr static uint32_t BUF_SIZE = 2048;
};