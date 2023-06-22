#pragma once

#include <stdio.h>

#define TEST_PASS 1
#define TEST_FAIL -1

#define ASSERT(X) if (!(X)) return TEST_FAIL;

