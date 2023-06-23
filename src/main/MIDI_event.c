#include "MIDI_internal.h"

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
      MIDI_event->body = control_change_reader(MIDI_file);
      break;

    case 0b1100:
      event->type = PROGRAM_CHANGE;
      MIDI_event->body = program_change_reader(MIDI_file);
      break;

    case 0b1101:
      event->type = CHANNEL_PRESSURE;
      MIDI_event->body = channel_pressure_reader(MIDI_file);
      break;

    case 0b1110:
      event->type = PITCH_WHEEL_CHANGE;
      MIDI_event->body = pitch_wheel_change_reader(MIDI_file);
      break;

    default:
      event = unimplemented_event_reader(MIDI_file);
  }

  return event;
}

struct note_toggle* note_toggle_reader(struct MIDI_file* MIDI_file) {
  struct note_toggle* note_toggle = (struct note_toggle*)malloc(sizeof(struct note_toggle));
  note_toggle->key = next_byte(MIDI_file) & 0b01111111;
  note_toggle->velocity = next_byte(MIDI_file) & 0b01111111;
  return note_toggle;
}

struct control_change* control_change_reader(struct MIDI_file* MIDI_file) {
  struct control_change* control_change = (struct control_change*)malloc(sizeof(struct control_change));
  control_change->controller = next_byte(MIDI_file) & 0b01111111;
  control_change->value = next_byte(MIDI_file) & 0b01111111;
  return control_change;
}

struct program_change* program_change_reader(struct MIDI_file* MIDI_file) {
  struct program_change* program_change = (struct program_change*)malloc(sizeof(struct program_change));
  program_change->program = next_byte(MIDI_file) & 0b01111111;
  return program_change;
}

struct channel_pressure* channel_pressure_reader(struct MIDI_file* MIDI_file) {
  struct channel_pressure* channel_pressure = (struct channel_pressure*)malloc(sizeof(struct channel_pressure));
  channel_pressure->pressure = next_byte(MIDI_file) & 0b01111111;
  return channel_pressure;
}

struct pitch_wheel_change* pitch_wheel_change_reader(struct MIDI_file* MIDI_file) {
  struct pitch_wheel_change* pitch_wheel_change = (struct pitch_wheel_change*)malloc(sizeof(struct pitch_wheel_change));
  pitch_wheel_change->value = next_byte(MIDI_file) & 0b01111111;
  pitch_wheel_change->value |= (next_byte(MIDI_file) & 0b01111111) << 7;
  return pitch_wheel_change;
}