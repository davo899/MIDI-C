#include "MIDI_event.h"

struct event* MIDI_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code) {
  struct event* event = (struct event*)malloc(sizeof(struct event));
  struct MIDI_event* MIDI_event = (struct MIDI_event*)malloc(sizeof(struct MIDI_event));
  MIDI_event->channel = event_code & 0x0F;
  event->body = MIDI_event;

  switch (event_code >> 4) {
    case 0b1000:
      event->type = NOTE_OFF;
      MIDI_event->body = note_toggle_reader(MIDI_file);
      break;

    case 0b1001:
      event->type = NOTE_ON;
      MIDI_event->body = note_toggle_reader(MIDI_file);
      break;

    case 0b1010:
      event->type = POLYPHONIC_KEY_PRESSURE;
      MIDI_event->body = note_toggle_reader(MIDI_file);
      break;

    default:
      event = unimplemented_event_reader(MIDI_file, event_code);
  }

  return event;
}

struct note_toggle* note_toggle_reader(struct MIDI_file* MIDI_file) {
  struct note_toggle* note_toggle = (struct note_toggle*)malloc(sizeof(struct note_toggle));
  note_toggle->key = next_byte(MIDI_file) & 0b01111111;
  note_toggle->velocity = next_byte(MIDI_file) & 0b01111111;
  return note_toggle;
}
