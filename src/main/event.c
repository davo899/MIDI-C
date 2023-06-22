#include "event.h"
#include "MIDI_event.h"
#include "meta_event.h"
#include "system_event.h"
#include <stdio.h>
#include <stdlib.h>

struct event* next_track_event(struct MIDI_file* MIDI_file, uint8_t event_code) {
  if (0x80 <= event_code && event_code < 0xF0) return MIDI_event_reader(MIDI_file, event_code);
  if (0xF0 <= event_code && event_code < 0xFF) return system_event_reader(MIDI_file, event_code);
  if (event_code == 0xFF)                      return meta_event_reader(MIDI_file, event_code);

  return unimplemented_event_reader(MIDI_file, event_code);
}

struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code) {
  return NULL;
}
