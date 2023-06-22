#include "MIDI_internal.h"

struct event* system_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code) {
  return unimplemented_event_reader(MIDI_file, event_code);
}
