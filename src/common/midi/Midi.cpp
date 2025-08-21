#include "Midi.h"
#include "common/Logger.h"

Midi::Midi()
{
    Pm_Initialize();
    updateDevices();
    //m_inputStream.deviceID = Pm_GetDefaultInputDeviceID();
    //m_outputStream.deviceID = Pm_GetDefaultInputDeviceID();
    setInput(true);
}

Midi::~Midi()
{
    Pm_Terminate();
}

bool Midi::handlePossibleError(const PmError err) const
{
    switch (err)
    {
    case pmNoError:
        return false;
        
    default:
        LOG_ERROR(Pm_GetErrorText(err));
        IM_ASSERT(true);
        return true; 
    }
}

void Midi::updateDevices()
{
    LOG_DEBUG("Updating midi-devices map");
    m_devices.clear();
    const PmDeviceID devicesCount = Pm_CountDevices();
    for (PmDeviceID i = 0; i < devicesCount; ++i)
    {
        const PmDeviceInfo* deviceInfo = Pm_GetDeviceInfo(i);
        if (deviceInfo == NULL) [[unlikely]]
        {
            LOG_ERROR("Updating midi-devices map. `Pm_GetDeviceInfo` returns `NULL`");
            return;
        }
        m_devices[i] = deviceInfo;

        // TODO: remove this:
        if (deviceInfo->input && (i == 1 || i ==3))
            m_inputStream.deviceID = i;
        LOG_DEBUG(std::format("Index: {}, Device name: {}", i, deviceInfo->name));
    }
}

const PmDeviceInfo* Midi::getSelectedInputDevice() const
{
    IM_ASSERT(m_devices.contains(m_inputStream.deviceID));
    return m_devices.at(m_inputStream.deviceID);
}

const PmDeviceInfo* Midi::getSelectedOutputDevice() const
{
    IM_ASSERT(m_devices.contains(m_outputStream.deviceID));
    return m_devices.at(m_outputStream.deviceID);
}

void Midi::setInputDevice(PmDeviceID newInputID)
{
    m_inputStream.deviceID = newInputID;
}

void Midi::setOutputDevice(PmDeviceID newOutputID)
{
    m_outputStream.deviceID = newOutputID;
}

void Midi::setInput(bool isEnabled /*= true*/)
{
    if (isEnabled)
    {
        // TODO: find a way to link some callback function to PortMidi `pm_read_short`. It is need to remove busy-waiting for AsyncMidiPoll thread.
        handlePossibleError(Pm_OpenInput(&m_inputStream.stream, m_inputStream.deviceID, nullptr, m_inputStream.BUF_SIZE, nullptr, nullptr));
        m_inputPoll = make::uptr<AsyncMidiPoll>(m_inputStream); // Works unltil we delete it
    }
    else
    {
        handlePossibleError(Pm_Close(m_inputStream.stream));
        m_inputStream.stream = nullptr;
        m_inputPoll = nullptr;
    }
}

void Midi::setOutput(bool isEnabled /*= true*/)
{
    
}
