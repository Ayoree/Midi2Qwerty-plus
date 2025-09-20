#pragma once
#include "MidiFile.h"

struct MidiFileEvent
{
    double time;
    smf::MidiEvent event;
};