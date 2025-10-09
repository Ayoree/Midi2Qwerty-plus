#pragma once
#include "MidiFile.h"

// Some small struct
struct MidiFileEvent
{
    double time;
    smf::MidiEvent event;
};