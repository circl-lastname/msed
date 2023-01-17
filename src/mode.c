#include <SDL.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "text.h"
#include "titlebar.h"
#include "mode.h"

#include "welcome.h"
#include "help.h"
#include "file.h"
#include "editor.h"

typedef struct {
  void (*enter)();
  void (*leave)();
  void (*handle_key)(SDL_Event* event);
  void (*handle_input)(SDL_Event* event);
  void (*draw)();
  
  char* text;
} mode_properties_t;

static unsigned char    top_bar[] = { 0xDA, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xBF, 0 };
static unsigned char bottom_bar[] = { 0xC0, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xD9, 0 };
static unsigned char   side_bar[] = { 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0xB3, 0x0A, 0 };

static mode_properties_t mode_properties[] = {
  { NULL, NULL, NULL, NULL, welcome_draw, "Welcome!" },
  { NULL, NULL, NULL, NULL, help_draw, "Help" },
  { file_enter, file_leave, file_handle_key, file_handle_input, file_draw, "File selection" },
  { NULL, NULL, NULL, NULL, editor_draw, "Preview" },
  { NULL, NULL, editor_select_handle_key, NULL, editor_draw, "Select" },
  { editor_bg_enter, NULL, editor_bg_handle_key, NULL, editor_draw, "Background" },
  { editor_fg_enter, NULL, editor_fg_handle_key, NULL, editor_draw, "Foreground" },
  { editor_char_enter, NULL, editor_char_handle_key, NULL, editor_draw, "Character" },
  { editor_copy_enter, NULL, editor_copy_handle_key, NULL, editor_draw, "Copy" },
  { editor_paste_enter, NULL, editor_paste_handle_key, NULL, editor_draw, "Paste" },
  { editor_text_enter, editor_text_leave, editor_text_handle_key, editor_text_handle_input, editor_draw, "Text entry" },
};

editor_mode_t current_mode = MODE_WELCOME;
static bool mode_selection = false;

void mode_handle_key(SDL_Event* event) {
  if (!mode_selection) {
    if (event->key.keysym.sym == SDLK_ESCAPE) {
      mode_selection = true;
    } else {
      if (mode_properties[current_mode].handle_key) {
        mode_properties[current_mode].handle_key(event);
      }
    }
  } else {
    if (mode_properties[current_mode].leave) {
      mode_properties[current_mode].leave();
    }
    
    switch (event->key.keysym.sym) {
      case SDLK_q:
        main_quit();
      break;
      case SDLK_h:
        current_mode = MODE_HELP;
      break;
      case SDLK_f:
        current_mode = MODE_FILE;
      break;
      case SDLK_o:
        current_mode = MODE_PREVIEW;
      break;
      case SDLK_s:
        current_mode = MODE_SELECT;
      break;
      case SDLK_b:
        current_mode = MODE_BG;
      break;
      case SDLK_n:
        current_mode = MODE_FG;
      break;
      case SDLK_m:
        current_mode = MODE_CHAR;
      break;
      case SDLK_c:
        current_mode = MODE_COPY;
      break;
      case SDLK_p:
        current_mode = MODE_PASTE;
      break;
      case SDLK_t:
        current_mode = MODE_TEXT;
      break;
    }
    
    if (mode_properties[current_mode].enter) {
      mode_properties[current_mode].enter();
    }
    
    mode_selection = false;
  }
  
  main_draw();
}

void mode_handle_input(SDL_Event* event) {
  // A mode which does not support it should never receive it
  mode_properties[current_mode].handle_input(event);
  
  main_draw();
}

void mode_draw() {
  SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
  SDL_Rect rect = { 0, 14, 656, 14 };
  SDL_RenderFillRect(renderer, &rect);
  
  text_draw(8, 14, (SDL_Color){ 0, 0, 0 }, mode_selection ? "Press a key to enter a mode" : mode_properties[current_mode].text);
  
  text_draw(8*39, 14, (SDL_Color){ 128, 128, 128 }, "\xB3");
  
  text_draw(0, 14*2, (SDL_Color){ 255, 255, 255 }, top_bar);
  text_draw(0, 14*3, (SDL_Color){ 255, 255, 255 }, side_bar);
  text_draw(656 - 8, 14*3, (SDL_Color){ 255, 255, 255 }, side_bar);
  text_draw(0, 392 - 14, (SDL_Color){ 255, 255, 255 }, bottom_bar);
  
  mode_properties[current_mode].draw();
}
