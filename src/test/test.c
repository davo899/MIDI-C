#include "test.h"
#include "tests.h"
#include <stddef.h>

int tests_length = sizeof(tests) / sizeof(struct test);

int main() {
  int fail_count = 0;

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
      fail_count++;
    }
    printf("\n");
  }
  if (fail_count > 0) {
    printf("\033[0m");
    printf("\n%d tests ", fail_count);
    printf("\033[0;31m");
    printf("failed");
  }
  printf("\033[0m");
}
