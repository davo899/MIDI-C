#pragma once

#include "event.h"

struct event* system_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);
