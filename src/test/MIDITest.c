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
  return read_file_to_byte_array("non-existent-file", &bytes_length) == NULL ? TEST_PASS : TEST_FAIL;
}
