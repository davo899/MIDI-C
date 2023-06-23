#include "MIDI_internal.h"

struct event* meta_event_reader(struct MIDI_file* MIDI_file) {
  return unimplemented_event_reader(MIDI_file, event_code);
}
