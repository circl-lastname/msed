#pragma once

#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  unsigned x;
  unsigned y;
  
  unsigned cursor;
  bool cursor_show;
  
  void (*handle_change)();
  
  size_t capacity;
  size_t length;
  
  char text[];
} textbox_t;

void textbox_handle_key(textbox_t* textbox, SDL_Event* event);
void textbox_handle_input(textbox_t* textbox, SDL_Event* event);
void textbox_draw(textbox_t* textbox);
