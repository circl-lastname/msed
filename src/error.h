#pragma once

#include <stdlib.h>

#define HE(...) \
  if (__VA_ARGS__) { \
    abort(); \
  } \
  macro_expect_semicolon()

inline static void macro_expect_semicolon() {};
