#include "MIDI_internal.h"

struct event* next_track_event(struct MIDI_file* MIDI_file, uint8_t event_code) {
  struct event* event = NULL;
  if      (0x80 <= event_code && event_code < 0xF0) event = MIDI_event_reader(MIDI_file, event_code);
  else if (0xF0 <= event_code && event_code < 0xFF) event = system_event_reader(MIDI_file, event_code);
  else if (event_code == 0xFF)                      event = meta_event_reader(MIDI_file);

  else event = unimplemented_event_reader(MIDI_file, event_code);
  return event;
}

struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code) {
  printf("Error: encountered unknown event code 0x%X", event_code);
  exit(1);
}
