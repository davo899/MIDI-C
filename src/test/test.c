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

  { .name = "next_byte advances index", .function = &next_byte_advances_index },
  { .name = "next_byte returns next byte", .function = &next_byte_returns_next_byte },
  { .name = "next_byte returns 0 past max index", .function = &next_byte_returns_zero_past_max_index },
};
int tests_length = sizeof(tests) / sizeof(struct test);

int main() {
  printf("\nRunning tests...\n");
  for (int i = 0; i < tests_length; i++) {
    printf("\033[0;37m");
    printf("%s: ", tests[i].name);
    if (tests[i].function() == TEST_PASS) {
      printf("\033[0;32m");
      printf("passed");
    } else {
      printf("\033[0;31m");
      printf("failed");
    }
    printf("\n");
  }
  printf("\033[0m");
}
