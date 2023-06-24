#include "MIDI_internal.h"

struct event* system_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code) {
  struct event* event = (struct event*)malloc(sizeof(struct event));

  switch (event_code) {
    case 0xF0:
      event->type = SYSTEM_EXCLUSIVE;
      struct system_exclusive_message* system_exclusive_message = (struct system_exclusive_message*)malloc(sizeof(struct system_exclusive_message));
      system_exclusive_message->id = next_byte(MIDI_file) & 0b01111111;
      system_exclusive_message->length = 0;
      while (MIDI_file->bytes[MIDI_file->index + system_exclusive_message->length] != 0xF7) system_exclusive_message->length++;
      system_exclusive_message->message = (uint8_t*)malloc(system_exclusive_message->length * sizeof(uint8_t));
      for (int i = 0; i < system_exclusive_message->length; i++) system_exclusive_message->message[i] = next_byte(MIDI_file) & 0b01111111;
      event->body = system_exclusive_message;
      break;

    default:
      free(event);
      event = unimplemented_event_reader(MIDI_file);
  }

  return event;
}
