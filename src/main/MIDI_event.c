#include "MIDI_internal.h"

static struct note_toggle* note_toggle_reader(struct MIDI_file* MIDI_file) {
  struct note_toggle* note_toggle = (struct note_toggle*)malloc(sizeof(struct note_toggle));
  note_toggle->key = next_byte(MIDI_file) & 0b01111111;
  note_toggle->velocity = next_byte(MIDI_file) & 0b01111111;
  return note_toggle;
}

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

    case 0b1011:
      event->type = CONTROL_CHANGE;
      struct control_change* control_change = (struct control_change*)malloc(sizeof(struct control_change));
      control_change->controller = next_byte(MIDI_file) & 0b01111111;
      control_change->value = next_byte(MIDI_file) & 0b01111111;
      MIDI_event->body = control_change;
      break;

    case 0b1100:
      event->type = PROGRAM_CHANGE;
      MIDI_event->body = malloc(sizeof(uint8_t));
      *(uint8_t*)MIDI_event->body = next_byte(MIDI_file) & 0b01111111;
      break;

    case 0b1101:
      event->type = CHANNEL_PRESSURE;
      MIDI_event->body = malloc(sizeof(uint8_t));
      *(uint8_t*)MIDI_event->body = next_byte(MIDI_file) & 0b01111111;
      break;

    case 0b1110:
      event->type = PITCH_WHEEL_CHANGE;
      MIDI_event->body = malloc(sizeof(uint16_t));
      uint16_t least_significant_byte = next_byte(MIDI_file) & 0b01111111;
      *(uint16_t*)MIDI_event->body = ((next_byte(MIDI_file) & 0b01111111) << 7) | least_significant_byte;
      break;

    default:
      free(event);
      event = unimplemented_event_reader(MIDI_file, event_code);
  }

  return event;
}
