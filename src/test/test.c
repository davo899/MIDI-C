#include "test.h"
#include <stdio.h>
#include <stddef.h>
#include "MIDITest.h"

struct test {
  char* name;
  int (*function)();
};

struct test tests[] = {
  { .name = "Can read file to byte array", .function = &can_read_file_to_byte_array }
};
int tests_length = sizeof(tests) / sizeof(struct test);

int main() {
  printf("\nRunning tests...\n");
  for (int i = 0; i < tests_length; i++) {
    if (tests[i].function() == TEST_PASS) printf("%s: passed\n", tests[i].name);
    else                                  printf("%s: failed\n", tests[i].name);
  }
}
