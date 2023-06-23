#include "MIDI_internal.h"

struct event* meta_event_reader(struct MIDI_file* MIDI_file) {
  struct event* event = (struct event*)malloc(sizeof(struct event));
  switch (next_byte(MIDI_file)) {
    case 0x00:
      event->type = SEQUENCE_NUMBER;
      next_byte(MIDI_file); // Always 0x02
      event->body = malloc(sizeof(uint16_t));
      *((uint16_t*)event->body) = next_half_word(MIDI_file);
      break;

    default:
      event = unimplemented_event_reader(MIDI_file);
  }

  return event;
}
