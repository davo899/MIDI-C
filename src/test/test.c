#include "test.h"
#include "tests.h"
#include <stddef.h>

struct test_group *test_groups[] = {
  &MIDI_tests,
  &MIDI_event_tests
};
int test_groups_length = sizeof(test_groups) / sizeof(struct test_group*);

int main() {
  int fail_count = 0;

  printf("\nRunning tests...\n");
  for (int i = 0; i < test_groups_length; i++) {
    printf("\033[0;37m");
    printf("%s\n", test_groups[i]->name);
    for (int j = 0; j < test_groups[i]->size; j++) {
      struct test test = test_groups[i]->tests[j];
      printf("\033[0;37m");
      printf("\t%s: ", test.name);
      if (test.function() == TEST_PASS) {
        printf("\033[0;32m");
        printf("passed");
      } else {
        printf("\033[0;31m");
        printf("failed");
        fail_count++;
      }
      printf("\n");
    }
  }
  if (fail_count > 0) {
    printf("\033[0m");
    printf("\n%d tests ", fail_count);
    printf("\033[0;31m");
    printf("failed");
  }
  printf("\033[0m");
}
