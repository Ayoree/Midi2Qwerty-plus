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

void AsyncMidiPoll::startPoll(std::stop_token stoken)
{
    using namespace std::chrono;
    LOG_DEBUG("AsyncMidiPoll thread started");
    LOG_DEBUG(std::format("Device id: {}", static_cast<int>(m_stream.deviceID)));

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
        LOG_ERROR("Unexpected error occured during midi stream reading:");
        LOG_ERROR(Pm_GetErrorText(error));
        m_workerThread.request_stop();
        return;
    }
    for (int ev = 0; ev < count; ev++) {
        //PmTimestamp timestamp = m_stream.buffer[ev].timestamp;
        PmMessage& message = m_stream.buffer[ev].message;
        uint8_t status = ((uint32_t) message & 0xFFu);
        PmMessage keyCode = (((uint32_t) message >> 8) & 0xFFu);
        PmMessage velocity = ((uint32_t) message >> 16 & 0xFFu);
        if (keyCode || velocity) {
            m_lastEventTime = std::chrono::steady_clock::now();
            if (status != 176) // not sustain
            {
                LOG_DEBUG(std::format("Event status: {}, key: {:04X}, velocity: {:04X}", status, keyCode, velocity));
                uint8_t keyIndex = PianoUtils::code2Key(keyCode);
                if (velocity != 0x00) // Press
                    PianoWindow::instance().pressKey(keyIndex);
                else // Release
                    PianoWindow::instance().releaseKey(keyIndex);
            }
        }
    }
}
