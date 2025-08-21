#include "Midi.h"
#include "common/Logger.h"

Midi::Midi()
{
    Pm_Initialize();
    updateDevices();
    m_selectedInputID = Pm_GetDefaultInputDeviceID();
    m_selectedOutputID = Pm_GetDefaultInputDeviceID();
}

Midi::~Midi()
{
    Pm_Terminate();
}

void Midi::updateDevices()
{
    LOG_DEBUG("Updating midi-devices map");
    m_devices.clear();
    const PmDeviceID devicesCount = Pm_CountDevices();
    for (PmDeviceID i = 0; i < devicesCount; ++i)
    {
        const PmDeviceInfo* deviceInfo = Pm_GetDeviceInfo(i);
        m_devices[i] = deviceInfo;
        LOG_DEBUG(std::format("Index: {}, Device name: {}", i, deviceInfo->name));
    }
}

const PmDeviceInfo* Midi::getSelectedInputDevice() const
{
    IM_ASSERT(m_devices.contains(m_selectedInputID));
    return m_devices.at(m_selectedInputID);
}

const PmDeviceInfo* Midi::getSelectedOutputDevice() const
{
    IM_ASSERT(m_devices.contains(m_selectedInputID));
    return m_devices.at(m_selectedOutputID);
}

void Midi::setInputDevice(PmDeviceID newInputID)
{
    m_selectedInputID = newInputID;
}

void Midi::setOutputDevice(PmDeviceID newOutputID)
{
    m_selectedOutputID = newOutputID;
}

void Midi::setInput(bool isEnabled /*= true*/)
{
    if (!isEnabled || m_inputStream != nullptr)
    {
        Pm_Close(m_inputStream);
    }
    if (isEnabled)
    {
        Pm_OpenInput(m_inputStream, m_selectedInputID, nullptr, )
    }
}

void Midi::setOutput(bool isEnabled /*= true*/)
{
    
}
