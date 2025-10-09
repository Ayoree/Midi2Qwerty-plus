#include "Midi.h"
#include "common/Logger.h"
#include "MidiFile.h"
#include "MidiFileEvent.h"
#include "windows/PianoWindow/PianoWindow.h"

using namespace smf;

Midi::Midi()
{
    handlePossibleError(Pm_Initialize());
    updateDevices();
    m_inputStream.deviceID = Pm_GetDefaultInputDeviceID();
    m_outputStream.deviceID = pmNoDevice;
    if (m_inputStream.deviceID != pmNoDevice)
        setInputState(true);
}

Midi::~Midi()
{
    handlePossibleError(Pm_Terminate());
}

bool Midi::handlePossibleError(const PmError err) const
{
    switch (err)
    {
    case pmNoError:
        return false;
        
    default:
        IM_ASSERT(true);
        LOG_ERROR(Pm_GetErrorText(err));
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
        LOG_DEBUG(std::format("Index: {}, Device name: {}", i, deviceInfo->name));
    }
}

const PmDeviceInfo* Midi::getSelectedInputDevice() const
{
    if (m_inputStream.deviceID == pmNoDevice) [[unlikely]]
    {
        LOG_ERROR("No selected MIDI input device");
        return nullptr;
    }
    IM_ASSERT(m_devices.contains(m_inputStream.deviceID));
    return m_devices.at(m_inputStream.deviceID);
}

const PmDeviceInfo* Midi::getSelectedOutputDevice() const
{
    if (m_outputStream.deviceID == pmNoDevice) [[unlikely]]
    {
        LOG_INFO("No selected MIDI output device");
        return nullptr;
    }
    IM_ASSERT(m_devices.contains(m_outputStream.deviceID));
    return m_devices.at(m_outputStream.deviceID);
}

void Midi::setInputDevice(PmDeviceID newInputID)
{
    if (newInputID != pmNoDevice && (!m_devices.contains(newInputID) || !m_devices.at(newInputID)->input))
    {
        LOG_ERROR(std::format("No available input device with id: {}", newInputID));
        return;
    }
    if (newInputID == m_inputStream.deviceID)
        return;
    setInputState(false);
    m_inputStream.deviceID = newInputID;
    if (newInputID != pmNoDevice)
        setInputState(true);
}

void Midi::setOutputDevice(PmDeviceID newOutputID)
{
    if (newOutputID != pmNoDevice && (!m_devices.contains(newOutputID) || !m_devices.at(newOutputID)->output))
    {
        LOG_ERROR(std::format("No available output device with id: {}", newOutputID));
        return;
    }
    if (newOutputID == m_outputStream.deviceID)
        return;

    setOutputState(false);
    m_outputStream.deviceID = newOutputID;
    if (newOutputID != pmNoDevice)
        setOutputState(true);
}

void Midi::openMidiFile(const std::string& filename)
{
    if (filename.empty())
    {
        LOG_ERROR("Filename is empty");
        return;
    }
    MidiFile midifile;
    midifile.read("./songs/" + filename);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    
    std::deque<MidiFileEvent> events;
    int tracks = midifile.getTrackCount();
    for (int track = 0; track < tracks; track++) {
        for (int event = 0; event < midifile[track].size(); event++) {
            MidiEvent& midiEvent = midifile[track][event];
            if (midiEvent.size() == 0 || midiEvent.isMetaMessage()) {
                continue;
            }      
            events.emplace_back(midiEvent.seconds, midiEvent);
        }
    }

    std::sort(events.begin(), events.end(), [](const MidiFileEvent& e1, const MidiFileEvent& e2) {
        return e1.time < e2.time;
    });

    m_midiPlayer = std::make_unique<MidiPlayer>(std::move(events));
    m_midiPlayer->setOutputStream(m_outputStream.stream);
}

void Midi::closeMidiFile()
{
    m_midiPlayer = nullptr;
}

void Midi::playMidiFile() const
{
    if (m_midiPlayer)
        m_midiPlayer->start();
    else
        LOG_ERROR("No midi-file selected");
}

void Midi::stopMidiFile()
{
    if (m_midiPlayer)
        m_midiPlayer->stop();
    PianoWindow::instance().releaseAll();
}

void Midi::setInputState(bool isEnabled /*= true*/)
{
    if (isEnabled)
    {
        // TODO: find a way to link some callback function to PortMidi `pm_read_short`. It is needed to remove busy-waiting for AsyncMidiPoll thread.
        handlePossibleError(Pm_OpenInput(&m_inputStream.stream, m_inputStream.deviceID, nullptr, m_inputStream.BUF_SIZE, nullptr, nullptr));
        m_inputPoll = make::uptr<AsyncMidiPoll>(m_inputStream); // Works in other thread unltil we delete it
        if (m_outputStream.stream)
            m_inputPoll->setOutputStream(m_outputStream.stream);
    }
    else
    {
        if (m_inputStream.stream)
            handlePossibleError(Pm_Close(m_inputStream.stream));
        m_inputStream.stream = nullptr;
        m_inputPoll = nullptr;
    }
}

void Midi::setOutputState(bool isEnabled /*= true*/)
{
    if (isEnabled)
    {
        handlePossibleError(Pm_OpenOutput(&m_outputStream.stream, m_outputStream.deviceID, nullptr, m_outputStream.BUF_SIZE, nullptr, nullptr, 0));
        if (m_inputPoll)
            m_inputPoll->setOutputStream(m_outputStream.stream);
        if (m_midiPlayer)
            m_midiPlayer->setOutputStream(m_outputStream.stream);
    }
    else
    {
        if (m_inputPoll)
            m_inputPoll->setOutputStream(nullptr);
        if (m_midiPlayer)
            m_midiPlayer->setOutputStream(nullptr);
        if (m_outputStream.stream)
            handlePossibleError(Pm_Close(m_outputStream.stream));
        m_outputStream.stream = nullptr;
    }
}
