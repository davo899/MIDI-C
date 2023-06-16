#include "test.h"
#include "../main/MIDI.h"
#include <stddef.h>

int can_read_file_to_byte_array() {
  uint8_t test_bytes[] = {
    0x61, 0x62, 0x63, 0x64, 0x68
  };
  int test_bytes_length = sizeof(test_bytes) / sizeof(uint8_t);

  int bytes_length;
  uint8_t* bytes = read_file_to_byte_array("src/test/test.txt", &bytes_length);
  if (bytes_length != test_bytes_length) return TEST_FAIL;

  for (int i = 0; i < test_bytes_length; i++) {
    if (bytes[i] != test_bytes[i]) return TEST_FAIL;
  }
  return TEST_PASS;
}

int reading_from_non_existent_file_returns_null() {
  int bytes_length;
  return assert(read_file_to_byte_array("non-existent-file", &bytes_length) == NULL);
}

int can_read_MIDI_file() {
  read_MIDI_file("test.mid");
  return TEST_PASS;
}

int can_read_VLQ_zero() {
  uint8_t bytes[] = { 0x00 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 1
  };
  return assert(next_variable_length_quantity(&MIDI_file) == 0 && MIDI_file.index == 1);
}

int can_read_VLQ_single() {
  uint8_t bytes[] = { 0x7B };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 1
  };
  return assert(next_variable_length_quantity(&MIDI_file) == 0x7B && MIDI_file.index == 1);
}

int can_read_VLQ_full() {
  uint8_t bytes[] = { 0xFF, 0xFF, 0xFF, 0x7F };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 4
  };
  return assert(next_variable_length_quantity(&MIDI_file) == 0xFFFFFFF && MIDI_file.index == 4);
}

int matching_chunk_type_returns_true() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 4
  };
  return assert(match_chunk_type(&MIDI_file, "MThd"));
}

int non_matching_chunk_type_returns_false() {
  uint8_t bytes[] = { 0x4D, 0x54, 0x68, 0x64 };
  struct MIDI_file MIDI_file = {
    .bytes = bytes,
    .index = 0,
    .length = 4
  };
  return assert(!match_chunk_type(&MIDI_file, "MTrk"));
}
