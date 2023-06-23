#pragma once

#include <stdio.h>

#define TEST_PASS 1
#define TEST_FAIL -1

#define ASSERT(X) if (!(X)) return TEST_FAIL;
#define INIT_TEST_GROUP(NAME) struct test_group NAME##_tests = { .name = #NAME, .tests = tests, .size = (sizeof(tests) / sizeof(struct test)) }
#define INIT_MIDI_FILE(NAME) \
  struct MIDI_file NAME = { \
    .bytes = bytes,\
    .index = 0,\
    .length = sizeof(bytes) / sizeof(uint8_t)\
  }

struct test {
  char* name;
  int (*function)();
};

struct test_group {
  char* name;
  struct test* tests;
  int size;
};
