#include "MidiPlayer.h"
#include "porttime.h"
#include "windows/PianoWindow/PianoWindow.h"

MidiPlayer::MidiPlayer(std::deque<MidiFileEvent> events) :
    m_events(std::move(events))
{}

MidiPlayer::~MidiPlayer()
{
    if (m_workerThread)
    {
        m_workerThread->request_stop();
        m_workerThread->join();
    }
}

void MidiPlayer::setOutputStream(PortMidiStream* stream)
{
    m_outputStream.store(stream);
}

void MidiPlayer::start()
{
    m_isRunning.store(true);
    m_startTime = Pt_Time() / 1000.0 - m_lastEventTime;
    m_workerThread = std::make_unique<std::jthread>(std::bind_front(&MidiPlayer::startPoll, this));
}

void MidiPlayer::stop()
{
    m_isRunning.store(false);
    if (m_workerThread)
        m_workerThread->request_stop();
    m_workerThread = nullptr;
}

void MidiPlayer::startPoll(std::stop_token stoken)
{
    while (!m_isRunning.load() && !stoken.stop_requested());
    while (!m_events.empty() && !stoken.stop_requested()) {
        std::unique_lock<std::mutex> lock(m_mtx);
        
        const double& nextTime = m_events.front().time;
        const MidiEvent& event = m_events.front().event;
        double currentTime = (Pt_Time() / 1000.0) - m_startTime;

        if (currentTime >= nextTime) {
            lock.unlock();
            m_lastEventTime = nextTime;

            PmMessage message = {0};
            for (size_t i = 0; i < event.size() && i < 4; ++i) {
                message |= (event[i] << (8 * i));
            }
            if (m_outputStream)
                Pm_WriteShort(m_outputStream, 0, message);

            if (event.isNote())
            {
                int keyIndex = event.getKeyNumber() - 21; // for 88-keys keyboard
                if (event.isNoteOn())
                    PianoWindow::instance().pressKey(keyIndex);
                else if (event.isNoteOff())
                    PianoWindow::instance().releaseKey(keyIndex);
            }
            else if (event.isSustain())
            {
                if (event.isSustainOn())
                    PianoKey::pressSpace();
                else if (event.isSustainOff())
                    PianoKey::releaseSpace();
            }
            
            m_events.pop_front();
            lock.lock();
        } else {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    stop();
}
