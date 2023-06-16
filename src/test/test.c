#include "test.h"
#include <stdio.h>
#include <stddef.h>
#include "MIDITest.h"

struct test {
  char* name;
  int (*function)();
};

struct test tests[] = {
  { .name = "Reading from non existent file returns null", .function = &reading_from_non_existent_file_returns_null },
  { .name = "Can read file to byte array", .function = &can_read_file_to_byte_array },

  { .name = "Can read MIDI file", .function = &can_read_MIDI_file },

  { .name = "Can read VLQ 0", .function = &can_read_VLQ_zero },
  { .name = "Can read 1 byte VLQ", .function = &can_read_VLQ_single },
  { .name = "Can read 4 byte VLQ", .function = &can_read_VLQ_full },

  { .name = "Matching chunk type returns true", .function = &matching_chunk_type_returns_true },
  { .name = "Non-matching chunk type returns false", .function = &non_matching_chunk_type_returns_false },
};
int tests_length = sizeof(tests) / sizeof(struct test);

int main() {
  printf("\nRunning tests...\n");
  for (int i = 0; i < tests_length; i++) {
    printf("%s: %s\n", tests[i].name, tests[i].function() == TEST_PASS ? "passed" : "failed");
  }
}

int assert(bool pass) {
  return pass ? TEST_PASS : TEST_FAIL;
}
