#include "AsyncMidiPoll.h"
#include "windows/PianoWindow/PianoWindow.h"

AsyncMidiPoll::AsyncMidiPoll(MidiStream& stream) :
    m_stream(stream),
    m_workerThread(std::bind_front(&AsyncMidiPoll::startPoll, this)),
    m_lastEventTime(std::chrono::steady_clock::now())
{}

AsyncMidiPoll::~AsyncMidiPoll()
{
    m_workerThread.request_stop();
    m_workerThread.join();
}

void AsyncMidiPoll::setOutputStream(PortMidiStream* stream)
{
    m_outputStream.store(stream);
}

void AsyncMidiPoll::startPoll(std::stop_token stoken)
{
    using namespace std::chrono;
    LOG_DEBUG("AsyncMidiPoll thread started");

    while (!stoken.stop_requested()) {
        readStreamData();
        nanoseconds timeFromLastEvent = steady_clock::now() - m_lastEventTime;
        std::this_thread::sleep_for(timeFromLastEvent > LONG_SLEEP_TIME_MARGIN ? LONG_SLEEP_TIME : SLEEP_TIME);
    }
    LOG_DEBUG("AsyncMidiPoll thread stopped");
}

void AsyncMidiPoll::readStreamData()
{
    int count = Pm_Read(m_stream.stream, m_stream.buffer, m_stream.BUF_SIZE);
    if (count < 0) [[unlikely]]
    {
        PmError error = static_cast<PmError>(count);
        LOG_ERROR(std::format("Unexpected error occured during midi input stream reading:{}", Pm_GetErrorText(error)));
        m_workerThread.request_stop();
        return;
    }
    
    if (m_outputStream.load())
    {
        PmError error = Pm_Write(m_outputStream.load(), m_stream.buffer, count);
        if (error != pmNoError)
            LOG_ERROR(std::format("Unexpected error occured during midi output stream writing: {}", Pm_GetErrorText(error)));
    }

    for (int ev = 0; ev < count; ev++) {
        //PmTimestamp timestamp = m_stream.buffer[ev].timestamp;
        PmMessage& message = m_stream.buffer[ev].message;
        uint8_t status = ((uint32_t) message & 0xFFu);
        PmMessage keyCode = (((uint32_t) message >> 8) & 0xFFu);
        PmMessage velocity = ((uint32_t) message >> 16 & 0xFFu);
        if (keyCode || velocity) {
            m_lastEventTime = std::chrono::steady_clock::now();
            if (status == 0b10110000) // sustain
            {
                if (velocity != 0x00) // Press
                    PianoKey::pressSpace();
                else // Release
                    PianoKey::releaseSpace();
            }
            else
            {
                //LOG_DEBUG(std::format("Event status: {}, key: {:04X}, velocity: {:04X}", status, keyCode, velocity));
                uint8_t keyIndex = PianoKey::midiCode2KeyIndex(keyCode);
                if (velocity != 0x00) // Press
                    PianoWindow::instance().pressKey(keyIndex);
                else // Release
                    PianoWindow::instance().releaseKey(keyIndex);
            }
        }
    }
}
