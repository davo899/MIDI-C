#include "test.h"
#include "../main/MIDI_internal.h"

static int reads_sequence_number() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x00, 0x02, 0x8D, 0x32 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == SEQUENCE_NUMBER);
  ASSERT(*((uint16_t*)event->body) == 0x8D32)
  return TEST_PASS;
}

static int reads_text() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x01, 0x02, 0x68, 0x69 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == TEXT);
  struct text_buffer* text_buffer = (struct text_buffer*)event->body;
  ASSERT(text_buffer->length == 2);
  ASSERT(text_buffer->text[0] == 'h');
  ASSERT(text_buffer->text[1] == 'i');
  return TEST_PASS;
}

static struct test tests[] = {
  { .name = "Reads sequence number event", .function = &reads_sequence_number },
  { .name = "Reads text event", .function = &reads_text },
};

INIT_TEST_GROUP(meta_event);