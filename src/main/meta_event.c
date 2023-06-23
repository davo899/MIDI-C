#include "MIDI_internal.h"

static struct text_buffer* text_buffer_reader(struct MIDI_file* MIDI_file, int length) {
  struct text_buffer* text_buffer = (struct text_buffer*)malloc(sizeof(struct text_buffer));
  text_buffer->length = length;
  text_buffer->text = (char*)malloc(length * sizeof(char));
  for (int i = 0; i < length; i++) text_buffer->text[i] = (char)next_byte(MIDI_file);
  return text_buffer;
}

struct event* meta_event_reader(struct MIDI_file* MIDI_file) {
  struct event* event = (struct event*)malloc(sizeof(struct event));
  int event_code = next_byte(MIDI_file);
  int length = next_variable_length_quantity(MIDI_file);

  switch (event_code) {
    case 0x00:
      event->type = SEQUENCE_NUMBER;
      event->body = malloc(sizeof(uint16_t));
      *((uint16_t*)event->body) = next_half_word(MIDI_file);
      break;

    case 0x01:
      event->type = TEXT;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x02:
      event->type = COPYRIGHT_NOTICE;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    default:
      event = unimplemented_event_reader(MIDI_file);
  }

  return event;
}
