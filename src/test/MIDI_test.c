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
  read_MIDI_file("test.mid");
  return TEST_PASS;
}

static int can_read_VLQ_zero() {
  uint8_t bytes[] = { 0x00 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 1
  };
  ASSERT(next_variable_length_quantity(&MIDI_file) == 0 && MIDI_file.index == 1);
  return TEST_PASS;
}

static int can_read_VLQ_single() {
  uint8_t bytes[] = { 0x7B };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 1
  };
  ASSERT(next_variable_length_quantity(&MIDI_file) == 0x7B && MIDI_file.index == 1);
  return TEST_PASS;
}

static int can_read_VLQ_full() {
  uint8_t bytes[] = { 0xFF, 0xFF, 0xFF, 0x7F };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 4
  };
  ASSERT(next_variable_length_quantity(&MIDI_file) == 0xFFFFFFF && MIDI_file.index == 4);
  return TEST_PASS;
}

static int matching_chunk_type_returns_true() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 4
  };
  ASSERT(match_chunk_type(&MIDI_file, "MThd"));
  return TEST_PASS;
}

static int non_matching_chunk_type_returns_false() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 4
  };
  ASSERT(!match_chunk_type(&MIDI_file, "MTrk"));
  return TEST_PASS;
}

static int next_byte_advances_index() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 4
  };
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
  struct MIDI_file MIDI_file2 = {
    .bytes = bytes,
    .index = 0,
    .length = 14
  };
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
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 14
  };
  struct MIDI_header* MIDI_header = next_MIDI_header(&MIDI_file);
  ASSERT(MIDI_header != NULL);
  ASSERT(MIDI_header->format == MULTI_TRACK_INDEPENDENT);
  ASSERT(MIDI_header->tracks == 0x10);
  ASSERT(MIDI_header->division == 0x8002);
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
};

struct test_group MIDI_tests = { .name = "MIDI", .tests = tests, .size = TEST_GROUP_SIZE(tests) };
