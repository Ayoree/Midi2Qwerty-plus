#include "AsyncMidiPoll.h"
#include "windows/PianoWindow/PianoWindow.h"
#include "MidiFile.h"

using namespace smf;

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
        LOG_ERROR(std::format("Unexpected error occured during midi input stream reading: {}", Pm_GetErrorText(error)));
        m_workerThread.request_stop();
        return;
    }
    
    if (m_outputStream.load())
    {
        PmError error = Pm_Write(m_outputStream.load(), m_stream.buffer, count);
        if (error != pmNoError) [[unlikely]]
            LOG_ERROR(std::format("Unexpected error occured during midi output stream writing: {}", Pm_GetErrorText(error)));
    }

    for (int ev = 0; ev < count; ev++) {
        PmMessage& message = m_stream.buffer[ev].message;

        // Converting PmMessage to MidiEvent
       std::vector<unsigned char> bytes;
        for (int i = 0; i < 3; ++i) {
            bytes.push_back((message >> (8 * i)) & 0xFFu);
        }
        MidiEvent midiEvent;
        midiEvent.assign(bytes.begin(), bytes.end());

        m_lastEventTime = std::chrono::steady_clock::now();
        if (midiEvent.isSustainOn())
            PianoKey::pressSpace();
        else if (midiEvent.isSustainOff())
            PianoKey::releaseSpace();
        else
        {
            int keyIndex = midiEvent.getKeyNumber() - 21; // for 88-keys keyboard
            if (midiEvent.isNoteOn())
            {
                PianoKey::setVelocity(midiEvent.getVelocity());
                PianoWindow::instance().pressKey(keyIndex);
            }
            else if (midiEvent.isNoteOff() || midiEvent.getVelocity() == 0)
                PianoWindow::instance().releaseKey(keyIndex);
        }
    }
}
