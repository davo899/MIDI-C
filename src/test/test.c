#include "test.h"
#include "tests.h"
#include <stddef.h>

enum colour {
  RESET, RED, GREEN, WHITE
};

struct test_group *test_groups[] = {
  &MIDI_tests,
  &MIDI_event_tests
};
int test_groups_length = sizeof(test_groups) / sizeof(struct test_group*);

static void set_colour(enum colour colour) {
  switch (colour) {
    case RESET: printf("\033[0m");    break;
    case RED:   printf("\033[0;31m"); break;
    case GREEN: printf("\033[0;32m"); break;
    case WHITE: printf("\033[0;37m"); break;
  };
}

int main() {
  int fail_count = 0;

  printf("\nRunning tests...\n");
  for (int i = 0; i < test_groups_length; i++) {
    printf("%s\n", test_groups[i]->name);

    for (int j = 0; j < test_groups[i]->size; j++) {
      struct test test = test_groups[i]->tests[j];

      printf("\t%s: ", test.name);
      if (test.function() == TEST_PASS) {
        set_colour(GREEN);
        printf("passed");
      } else {
        set_colour(RED);
        printf("failed");
        fail_count++;
      }
      printf("\n");
      set_colour(WHITE);
    }
    printf("\n");
  }
  if (fail_count > 0) {
    set_colour(RESET);
    printf("\n%d tests ", fail_count);
    set_colour(RED);
    printf("failed");
  }
  set_colour(RESET);
}
