#include "MIDI_internal.h"

struct event* next_track_event(struct MIDI_file* MIDI_file) {
  uint32_t deltatime = next_variable_length_quantity(MIDI_file);
  uint8_t event_code = next_byte(MIDI_file);

  struct event* event = unimplemented_event_reader(MIDI_file);
  if      (0x80 <= event_code && event_code < 0xF0) event = MIDI_event_reader(MIDI_file, event_code);
  else if (0xF0 <= event_code && event_code < 0xFF) event = system_event_reader(MIDI_file, event_code);
  else if (event_code == 0xFF)                      event = meta_event_reader(MIDI_file);

  if (event != NULL) event->deltatime = deltatime;
  return event;
}

struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file) {
  return NULL;
}
