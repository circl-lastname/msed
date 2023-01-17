#pragma once

#include <SDL.h>

typedef enum {
  MODE_WELCOME,
  MODE_HELP,
  MODE_FILE,
  MODE_PREVIEW,
  MODE_SELECT,
  MODE_BG,
  MODE_FG,
  MODE_CHAR,
  MODE_COPY,
  MODE_PASTE,
  MODE_TEXT,
} editor_mode_t;

extern editor_mode_t current_mode;

void mode_handle_key(SDL_Event* event);
void mode_handle_input(SDL_Event* event);
void mode_draw();
