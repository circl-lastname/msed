#pragma once

#include <SDL.h>

#include "text.h"

#define TEXT_HELPER_INIT() \
  unsigned x = 1; \
  unsigned y = 3; \
  unsigned base = 1

#define TEXT_HELPER_SET_BASE(new_base) \
  base = new_base

#define TEXT_HELPER_MOVE(new_x, new_y) \
  x = new_x; \
  y = new_y

#define TEXT_HELPER_RETURN() \
  x = base; \
  y += 1

#define TEXT_HELPER_DRAW(text, r, g, b) \
  text_draw(8*x, 14*y, (SDL_Color){ r, g, b }, text); \
  x += sizeof(text) - 1
