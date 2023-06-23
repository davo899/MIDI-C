#pragma once

#include <stdio.h>

#define TEST_PASS 1
#define TEST_FAIL -1

#define ASSERT(X) if (!(X)) return TEST_FAIL;
#define TEST_GROUP_SIZE(X) (sizeof(X) / sizeof(struct test))

struct test {
  char* name;
  int (*function)();
};

struct test_group {
  char* name;
  struct test* tests;
  int size;
};
