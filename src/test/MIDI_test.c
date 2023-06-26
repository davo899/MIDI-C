#include "test.h"
#include "../main/MIDI_internal.h"

static int can_read_file_to_byte_array() {
  uint8_t test_bytes[] = { 0x61, 0x62, 0x63, 0x64, 0x68 };
  int test_bytes_length = sizeof(test_bytes) / sizeof(uint8_t);

  int bytes_length;
  uint8_t* bytes = read_file_to_byte_array("src/test/test.txt", &bytes_length);
  if (bytes_length != test_bytes_length) return TEST_FAIL;

  for (int i = 0; i < test_bytes_length; i++) {
    if (bytes[i] != test_bytes[i]) return TEST_FAIL;
  }
  return TEST_PASS;
}

static int reading_from_non_existent_file_returns_null() {
  int bytes_length;
  ASSERT(read_file_to_byte_array("non-existent-file", &bytes_length) == NULL);
  return TEST_PASS;
}

static int can_read_MIDI_file() {
  read_MIDI_file("src/test/test.mid");
  return TEST_PASS;
}

static int can_read_VLQ_zero() {
  uint8_t bytes[] = { 0x00 };
  INIT_MIDI_FILE(MIDI_file);
  ASSERT(next_variable_length_quantity(&MIDI_file) == 0 && MIDI_file.index == 1);
  return TEST_PASS;
}

static int can_read_VLQ_single() {
  uint8_t bytes[] = { 0x7B };
  INIT_MIDI_FILE(MIDI_file);
  ASSERT(next_variable_length_quantity(&MIDI_file) == 0x7B && MIDI_file.index == 1);
  return TEST_PASS;
}

static int can_read_VLQ_full() {
  uint8_t bytes[] = { 0xFF, 0xFF, 0xFF, 0x7F };
  INIT_MIDI_FILE(MIDI_file);
  ASSERT(next_variable_length_quantity(&MIDI_file) == 0xFFFFFFF && MIDI_file.index == 4);
  return TEST_PASS;
}

static int matching_chunk_type_returns_true() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  INIT_MIDI_FILE(MIDI_file);
  ASSERT(match_chunk_type(&MIDI_file, "MThd"));
  return TEST_PASS;
}

static int non_matching_chunk_type_returns_false() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  INIT_MIDI_FILE(MIDI_file);
  ASSERT(!match_chunk_type(&MIDI_file, "MTrk"));
  return TEST_PASS;
}

static int next_byte_advances_index() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  INIT_MIDI_FILE(MIDI_file);
  next_byte(&MIDI_file);
  ASSERT(MIDI_file.index == 1);
  return TEST_PASS;
}

static int next_byte_returns_next_byte() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 1,
    .length = 4
  };
  ASSERT(next_byte(&MIDI_file) == 0x54);
  return TEST_PASS;
}

static int next_byte_returns_zero_past_max_index() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 4,
    .length = 4
  };
  ASSERT(next_byte(&MIDI_file) == 0);
  return TEST_PASS;
}

static int invalid_header_returns_null() {
  struct MIDI_file MIDI_file1 = {
    .bytes = NULL,
    .index = 0,
    .length = 0
  };
  ASSERT(next_MIDI_header(&MIDI_file1) == NULL);

  uint8_t bytes[] = {
    0x4D, 0x54, 0x68, 0x64,
    0x00, 0x00, 0x00, 0x02,
    0x00, 0x01,
    0x00, 0x10,
    0x80, 0x02
  };
  INIT_MIDI_FILE(MIDI_file2);
  ASSERT(next_MIDI_header(&MIDI_file2) == NULL);

  return TEST_PASS;
}

static int can_read_header() {
  uint8_t bytes[] = {
    0x4D, 0x54, 0x68, 0x64,
    0x00, 0x00, 0x00, 0x06,
    0x00, 0x02,
    0x00, 0x10,
    0x80, 0x02
  };
  INIT_MIDI_FILE(MIDI_file);
  struct MIDI_header* MIDI_header = next_MIDI_header(&MIDI_file);
  ASSERT(MIDI_header != NULL);
  ASSERT(MIDI_header->format == MULTI_TRACK_INDEPENDENT);
  ASSERT(MIDI_header->tracks == 0x10);
  ASSERT(MIDI_header->division == 0x8002);
  return TEST_PASS;
}

static int can_read_track() {
  uint8_t bytes[] = {
    0x4D, 0x54, 0x72, 0x6B, // MTrk
    0x00, 0x00, 0x00, 0x0C, // Length
    0x68, 0x9A, 0x7F, 0x6B, // Note on
    0x12, 0x8A, 0x7F, 0x19, // Note off
    0x5B, 0xFF, 0x2F, 0x00  // Track end
  };
  INIT_MIDI_FILE(MIDI_file);
  struct MIDI_track* MIDI_track = next_MIDI_track(&MIDI_file);
  ASSERT(MIDI_track != NULL);
  ASSERT(MIDI_track->length == 3);

  ASSERT(MIDI_track->events[0]->deltatime == 0x68);
  ASSERT(MIDI_track->events[0]->type == NOTE_ON);
  ASSERT(((struct MIDI_event*)MIDI_track->events[0]->body)->channel == 0xA);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[0]->body)->body)->key == 0x7F);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[0]->body)->body)->velocity == 0x6B);

  ASSERT(MIDI_track->events[1]->deltatime == 0x12);
  ASSERT(MIDI_track->events[1]->type == NOTE_OFF);
  ASSERT(((struct MIDI_event*)MIDI_track->events[1]->body)->channel == 0xA);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[1]->body)->body)->key == 0x7F);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[1]->body)->body)->velocity == 0x19);

  ASSERT(MIDI_track->events[2]->deltatime == 0x5B);
  ASSERT(MIDI_track->events[2]->type == END_OF_TRACK);
  return TEST_PASS;
}

static int can_read_running_status() {
  uint8_t bytes[] = {
    0x4D, 0x54, 0x72, 0x6B, // MTrk
    0x00, 0x00, 0x00, 0x0C, // Length
    0x68, 0x90, 0x3C, 0x7F, // Note on
    0x00, 0x40, 0x7F,             // Note on
    0x01, 0x43, 0x7F,             // Note on
    0x5B, 0xFF, 0x2F, 0x00  // Track end
  };
  INIT_MIDI_FILE(MIDI_file);
  struct MIDI_track* MIDI_track = next_MIDI_track(&MIDI_file);
  ASSERT(MIDI_track != NULL);
  ASSERT(MIDI_track->length == 4);

  ASSERT(MIDI_track->events[0]->deltatime == 0x68);
  ASSERT(MIDI_track->events[0]->type == NOTE_ON);
  ASSERT(((struct MIDI_event*)MIDI_track->events[0]->body)->channel == 0x0);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[0]->body)->body)->key == 0x3C);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[0]->body)->body)->velocity == 0x7F);

  ASSERT(MIDI_track->events[1]->deltatime == 0x00);
  ASSERT(MIDI_track->events[1]->type == NOTE_ON);
  ASSERT(((struct MIDI_event*)MIDI_track->events[1]->body)->channel == 0x0);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[1]->body)->body)->key == 0x40);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[1]->body)->body)->velocity == 0x7F);

  ASSERT(MIDI_track->events[2]->deltatime == 0x01);
  ASSERT(MIDI_track->events[2]->type == NOTE_ON);
  ASSERT(((struct MIDI_event*)MIDI_track->events[2]->body)->channel == 0x0);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[2]->body)->body)->key == 0x43);
  ASSERT(((struct note_toggle*)((struct MIDI_event*)MIDI_track->events[2]->body)->body)->velocity == 0x7F);

  ASSERT(MIDI_track->events[3]->deltatime == 0x5B);
  ASSERT(MIDI_track->events[3]->type == END_OF_TRACK);
  return TEST_PASS;
}

static int can_read_MIDI() {
  read_MIDI("src/test/test.mid");
  return TEST_PASS;
}

static struct test tests[] = {
  { .name = "Reading from non existent file returns null", .function = &reading_from_non_existent_file_returns_null },
  { .name = "Can read file to byte array", .function = &can_read_file_to_byte_array },

  { .name = "Can read MIDI file", .function = &can_read_MIDI_file },

  { .name = "Can read VLQ 0", .function = &can_read_VLQ_zero },
  { .name = "Can read 1 byte VLQ", .function = &can_read_VLQ_single },
  { .name = "Can read 4 byte VLQ", .function = &can_read_VLQ_full },

  { .name = "Matching chunk type returns true", .function = &matching_chunk_type_returns_true },
  { .name = "Non-matching chunk type returns false", .function = &non_matching_chunk_type_returns_false },

  { .name = "next_byte advances index", .function = &next_byte_advances_index },
  { .name = "next_byte returns next byte", .function = &next_byte_returns_next_byte },
  { .name = "next_byte returns 0 past max index", .function = &next_byte_returns_zero_past_max_index },

  { .name = "Can read header", .function = &can_read_header },
  { .name = "Reading invalid header returns NULL", .function = &invalid_header_returns_null },
  { .name = "Can read track", .function = &can_read_track },
  { .name = "Can read running status", .function = &can_read_running_status },
  { .name = "Can read MIDI", .function = &can_read_MIDI },
};

INIT_TEST_GROUP(MIDI);
