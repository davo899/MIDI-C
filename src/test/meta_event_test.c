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

static int reads_copyright_notice() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x02, 0x02, 0x68, 0x69 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == COPYRIGHT_NOTICE);
  struct text_buffer* text_buffer = (struct text_buffer*)event->body;
  ASSERT(text_buffer->length == 2);
  ASSERT(text_buffer->text[0] == 'h');
  ASSERT(text_buffer->text[1] == 'i');
  return TEST_PASS;
}

static int reads_track_name() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x03, 0x02, 0x68, 0x69 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == TRACK_NAME);
  struct text_buffer* text_buffer = (struct text_buffer*)event->body;
  ASSERT(text_buffer->length == 2);
  ASSERT(text_buffer->text[0] == 'h');
  ASSERT(text_buffer->text[1] == 'i');
  return TEST_PASS;
}

static int reads_instrument_name() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x04, 0x02, 0x68, 0x69 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == INSTRUMENT_NAME);
  struct text_buffer* text_buffer = (struct text_buffer*)event->body;
  ASSERT(text_buffer->length == 2);
  ASSERT(text_buffer->text[0] == 'h');
  ASSERT(text_buffer->text[1] == 'i');
  return TEST_PASS;
}

static int reads_lyric() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x05, 0x02, 0x68, 0x69 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == LYRIC);
  struct text_buffer* text_buffer = (struct text_buffer*)event->body;
  ASSERT(text_buffer->length == 2);
  ASSERT(text_buffer->text[0] == 'h');
  ASSERT(text_buffer->text[1] == 'i');
  return TEST_PASS;
}

static int reads_marker() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x06, 0x02, 0x68, 0x69 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == MARKER);
  struct text_buffer* text_buffer = (struct text_buffer*)event->body;
  ASSERT(text_buffer->length == 2);
  ASSERT(text_buffer->text[0] == 'h');
  ASSERT(text_buffer->text[1] == 'i');
  return TEST_PASS;
}

static int reads_cue_point() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x07, 0x02, 0x68, 0x69 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == CUE_POINT);
  struct text_buffer* text_buffer = (struct text_buffer*)event->body;
  ASSERT(text_buffer->length == 2);
  ASSERT(text_buffer->text[0] == 'h');
  ASSERT(text_buffer->text[1] == 'i');
  return TEST_PASS;
}

static int reads_channel_prefix() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x20, 0x01, 0x07 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == CHANNEL_PREFIX);
  ASSERT(*(uint8_t*)event->body == 0x07);
  return TEST_PASS;
}

static int reads_end_of_track() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x2F, 0x00 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == END_OF_TRACK);
  return TEST_PASS;
}

static int reads_set_tempo() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x51, 0x03, 0x98, 0x76, 0x54 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == SET_TEMPO);
  ASSERT(*(uint64_t*)event->body == 0x987654);
  return TEST_PASS;
}

static int reads_SMPTE_offset() {
  uint8_t bytes[] = { 0x7C, 0xFF, 0x54, 0x05, 0x7F, 0xAF, 0xEE, 0x3D, 0xCC };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == SMPTE_OFFSET);
  struct SMPTE_offset* SMPTE_offset = (struct SMPTE_offset*)event->body;
  ASSERT(SMPTE_offset->frame_rate == 0x3);
  ASSERT(SMPTE_offset->hours == 0x1F);
  ASSERT(SMPTE_offset->minutes == 0xAF);
  ASSERT(SMPTE_offset->seconds == 0xEE);
  ASSERT(SMPTE_offset->frames == 0x3D);
  ASSERT(SMPTE_offset->subframes == 0xCC);
  return TEST_PASS;
}

static struct test tests[] = {
  { .name = "Reads sequence number event", .function = &reads_sequence_number },
  { .name = "Reads text event", .function = &reads_text },
  { .name = "Reads copyright notice event", .function = &reads_copyright_notice },
  { .name = "Reads track name event", .function = &reads_track_name },
  { .name = "Reads instrument name event", .function = &reads_instrument_name },
  { .name = "Reads lyric event", .function = &reads_lyric },
  { .name = "Reads marker event", .function = &reads_marker },
  { .name = "Reads cue point event", .function = &reads_cue_point },
  { .name = "Reads channel prefix event", .function = &reads_channel_prefix },
  { .name = "Reads end of track event", .function = &reads_end_of_track },
  { .name = "Reads set tempo event", .function = &reads_set_tempo },
  { .name = "Reads SMPTE offset event", .function = &reads_SMPTE_offset },
};

INIT_TEST_GROUP(meta_event);
