#pragma once

#include "event.h"

struct event* MIDI_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);

struct MIDI_event {
  int channel;
  void* body;
};

struct note_toggle {
  int key;
  int velocity;
};

struct note_toggle* note_toggle_reader(struct MIDI_file* MIDI_file);
