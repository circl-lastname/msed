#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "text.h"
#include "mode.h"

typedef uint8_t ansi_color_t;

enum {
  ANSI_BLACK,
  ANSI_RED,
  ANSI_GREEN,
  ANSI_YELLOW,
  ANSI_BLUE,
  ANSI_MAGENTA,
  ANSI_CYAN,
  ANSI_WHITE,
  ANSI_B_BLACK,
  ANSI_B_RED,
  ANSI_B_GREEN,
  ANSI_B_YELLOW,
  ANSI_B_BLUE,
  ANSI_B_MAGENTA,
  ANSI_B_CYAN,
  ANSI_B_WHITE,
};

static SDL_Color ansi_color[] = {
  { 0, 0, 0 },
  { 128, 0, 0 },
  { 0, 128, 0 },
  { 128, 128, 0 },
  { 0, 0, 128 },
  { 128, 0, 128 },
  { 0, 128, 128 },
  { 192, 192, 192 },
  { 128, 128, 128 },
  { 255, 0, 0 },
  { 0, 255, 0 },
  { 255, 255, 0 },
  { 0, 0, 255 },
  { 255, 0, 255 },
  { 0, 255, 255 },
  { 255, 255, 255 },
};

static char* ansi_bg_code[] = {
  "40",
  "41",
  "42",
  "43",
  "44",
  "45",
  "46",
  "47",
  "100",
  "101",
  "102",
  "103",
  "104",
  "105",
  "106",
  "107",
};

static char* ansi_fg_code[] = {
  "30",
  "31",
  "32",
  "33",
  "34",
  "35",
  "36",
  "37",
  "90",
  "91",
  "92",
  "93",
  "94",
  "95",
  "96",
  "97",
};

static unsigned char buffer[80][24];
static ansi_color_t bg_color[80][24] = { { 0 } };
static ansi_color_t fg_color[80][24];

static SDL_Rect cursor = { 0, 0, 1, 1 };

static bool select_drag = false;

static ansi_color_t current_color;
static unsigned char current_char;

static unsigned char copy_buffer[80][24] = { { ' ' } };
static ansi_color_t copy_bg_color[80][24] = { { ANSI_BLACK } };
static ansi_color_t copy_fg_color[80][24] = { { ANSI_B_WHITE } };
static unsigned copy_width = 1;
static unsigned copy_height = 1;
static bool copy_show_copied = false;

static unsigned text_base;

void editor_init() {
  // There's no standard way to fill an array with a certain value in C
  for (unsigned x = 0; x < 80; x++) {
    for (unsigned y = 0; y < 24; y++) {
      buffer[x][y] = ' ';
      fg_color[x][y] = ANSI_B_WHITE;
    }
  }
}

void editor_save(FILE* file) {
  fputs("\x1b[", file);
  fputs(ansi_bg_code[bg_color[0][0]], file);
  fputc(';', file);
  fputs(ansi_fg_code[fg_color[0][0]], file);
  fputc('m', file);
  
  ansi_color_t current_bg = bg_color[0][0];
  ansi_color_t current_fg = fg_color[0][0];
  
  for (unsigned y = 0; y < 24; y++) {
    for (unsigned x = 0; x < 80; x++) {
      if (current_bg != bg_color[x][y] && current_fg != fg_color[x][y]) {
        fputs("\x1b[", file);
        fputs(ansi_bg_code[bg_color[x][y]], file);
        fputc(';', file);
        fputs(ansi_fg_code[fg_color[x][y]], file);
        fputc('m', file);
        
        current_bg = bg_color[x][y];
        current_fg = fg_color[x][y];
      } else if (current_bg != bg_color[x][y]) {
        fputs("\x1b[", file);
        fputs(ansi_bg_code[bg_color[x][y]], file);
        fputc('m', file);
        
        current_bg = bg_color[x][y];
      } else if (current_fg != fg_color[x][y]) {
        fputs("\x1b[", file);
        fputs(ansi_fg_code[fg_color[x][y]], file);
        fputc('m', file);
        
        current_fg = fg_color[x][y];
      }
      
      fputc(buffer[x][y], file);
    }
    
    fputs("\r\n", file);
  }
  
  fputs("\x1b[0m", file);
}

void editor_load(FILE* file) {
  ansi_color_t current_bg = ANSI_BLACK;
  ansi_color_t current_fg = ANSI_B_WHITE;
  
  for (unsigned y = 0; y < 24; y++) {
    for (unsigned x = 0; x < 80; x++) {
      int ch = fgetc(file);
      
      if (ch == '\x1b') {
        fseek(file, 1, SEEK_CUR);
        
        while (true) {
          ch = fgetc(file);
          
          ansi_color_t* current = NULL;
          unsigned offset = 0;
          
          switch (ch) {
            default:
            case '4':
              current = &current_bg;
            break;
            case '1':
              current = &current_bg;
              offset = 8;
              fseek(file, 1, SEEK_CUR);
            break;
            case '3':
              current = &current_fg;
            break;
            case '9':
              current = &current_fg;
              offset = 8;
            break;
          }
          
          ch = fgetc(file);
          
          *current = (offset + ch - 0x30) % 16;
          
          ch = fgetc(file);
          
          if (ch == 'm') {
            break;
          }
        }
        
        ch = fgetc(file);
      }
      
      buffer[x][y] = ch;
      bg_color[x][y] = current_bg;
      fg_color[x][y] = current_fg;
    }
    
    fseek(file, 2, SEEK_CUR);
  }
}

static bool cursor_handle_key(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_UP:
      if (cursor.y != 0) {
        cursor.y--;
      }
    break;
    case SDLK_DOWN:
      if (cursor.y + cursor.h - 1 != 23) {
        cursor.y++;
      }
    break;
    case SDLK_LEFT:
      if (cursor.x != 0) {
        cursor.x--;
      }
    break;
    case SDLK_RIGHT:
      if (cursor.x + cursor.w - 1 != 79) {
        cursor.x++;
      }
    break;
    default:
      return false;
    break;
  }
  
  return true;
}

void editor_select_handle_key(SDL_Event* event) {
  if (event->key.keysym.sym == SDLK_RETURN) {
    if (select_drag) {
      cursor.w = 1;
      cursor.h = 1;
    }
    
    select_drag = !select_drag;
  }
  
  if (!select_drag) {
    cursor_handle_key(event);
  } else {
    switch (event->key.keysym.sym) {
      case SDLK_UP:
        if (cursor.h != 1) {
          cursor.h--;
        }
      break;
      case SDLK_DOWN:
        if (cursor.h - 1 + cursor.y != 23) {
          cursor.h++;
        }
      break;
      case SDLK_LEFT:
        if (cursor.w != 1) {
          cursor.w--;
        }
      break;
      case SDLK_RIGHT:
        if (cursor.w - 1 + cursor.x != 79) {
          cursor.w++;
        }
      break;
    }
  }
}

static void bg_fill() {
  for (unsigned x = cursor.x; x < cursor.x + cursor.w; x++) {
    for (unsigned y = cursor.y; y < cursor.y + cursor.h; y++) {
      bg_color[x][y] = current_color;
    }
  }
}

void editor_bg_enter() {
  if (cursor.w == 1 && cursor.h == 1) {
    current_color = bg_color[cursor.x][cursor.y];
  } else {
    current_color = ANSI_B_WHITE;
  }
}

void editor_bg_handle_key(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_MINUS:
      current_color = (current_color - 1) % 16;
      bg_fill();
    break;
    case SDLK_EQUALS:
      current_color = (current_color + 1) % 16;
      bg_fill();
    break;
  }
}

static void fg_fill() {
  for (unsigned x = cursor.x; x < cursor.x + cursor.w; x++) {
    for (unsigned y = cursor.y; y < cursor.y + cursor.h; y++) {
      fg_color[x][y] = current_color;
    }
  }
}

void editor_fg_enter() {
  if (cursor.w == 1 && cursor.h == 1) {
    current_color = fg_color[cursor.x][cursor.y];
  } else {
    current_color = ANSI_B_WHITE;
  }
}

void editor_fg_handle_key(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_MINUS:
      current_color = (current_color - 1) % 16;
      fg_fill();
    break;
    case SDLK_EQUALS:
      current_color = (current_color + 1) % 16;
      fg_fill();
    break;
  }
}

static void char_fill() {
  for (unsigned x = cursor.x; x < cursor.x + cursor.w; x++) {
    for (unsigned y = cursor.y; y < cursor.y + cursor.h; y++) {
      buffer[x][y] = current_char;
    }
  }
}

void editor_char_enter() {
  if (cursor.w == 1 && cursor.h == 1) {
    current_char = buffer[cursor.x][cursor.y];
  } else {
    current_char = ' ' - 1;
  }
}

void editor_char_handle_key(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_MINUS:
      current_char--;
      char_fill();
    break;
    case SDLK_EQUALS:
      current_char++;
      char_fill();
    break;
  }
}

void editor_copy_enter() {
  copy_show_copied = false;
}

void editor_copy_handle_key(SDL_Event* event) {
  if (event->key.keysym.sym == SDLK_RETURN) {
    for (unsigned x = 0; x < cursor.w; x++) {
      for (unsigned y = 0; y < cursor.h; y++) {
        copy_buffer[x][y] = buffer[cursor.x + x][cursor.y + y];
        copy_bg_color[x][y] = bg_color[cursor.x + x][cursor.y + y];
        copy_fg_color[x][y] = fg_color[cursor.x + x][cursor.y + y];
      }
    }
    
    copy_width = cursor.w;
    copy_height = cursor.h;
    
    copy_show_copied = true;
  } else if (cursor_handle_key(event)) {
    copy_show_copied = false;
  }
}

void editor_paste_enter() {
  cursor.w = copy_width;
  cursor.h = copy_height;
  
  if (cursor.x + cursor.w - 1 > 79) {
    cursor.x -= cursor.w - 1;
  }
  
  if (cursor.y + cursor.h - 1 > 23) {
    cursor.y -= cursor.h - 1;
  }
}

void editor_paste_handle_key(SDL_Event* event) {
  if (event->key.keysym.sym == SDLK_RETURN) {
    for (unsigned x = 0; x < cursor.w; x++) {
      for (unsigned y = 0; y < cursor.h; y++) {
        buffer[cursor.x + x][cursor.y + y] = copy_buffer[x][y];
        bg_color[cursor.x + x][cursor.y + y] = copy_bg_color[x][y];
        fg_color[cursor.x + x][cursor.y + y] = copy_fg_color[x][y];
      }
    }
  } else {
    cursor_handle_key(event);
  }
}

void editor_text_enter() {
  cursor.w = 1;
  cursor.h = 1;
  select_drag = false;
  
  text_base = cursor.x;
  
  SDL_StartTextInput();
}

void editor_text_leave() {
  SDL_StopTextInput();
}

void editor_text_handle_key(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_RETURN:
      cursor.x = text_base;
      
      if (cursor.y != 23) {
        cursor.y++;
      }
    break;
    case SDLK_BACKSPACE:
      if (cursor.x != 0) {
        cursor.x--;
      }
      
      buffer[cursor.x][cursor.y] = ' ';
    break;
    case SDLK_DELETE:
      if (cursor.x != 79) {
        cursor.x++;
      }
      
      buffer[cursor.x][cursor.y] = ' ';
    break;
    default:
      cursor_handle_key(event);
    break;
  }
}

void editor_text_handle_input(SDL_Event* event) {
  buffer[cursor.x][cursor.y] = event->text.text[0];
  
  if (cursor.x != 79) {
    cursor.x++;
  }
}

void editor_draw() {
  if (current_mode != MODE_PREVIEW) {
    text_draw(8*40, 14, (SDL_Color){ 0, 0, 0 }, "X=   Y=   W=   H=");
    
    char coord[3];
    
    snprintf(coord, 3, "%u", cursor.x);
    text_draw(8*42, 14, (SDL_Color){ 0, 0, 0 }, coord);
    
    snprintf(coord, 3, "%u", cursor.y);
    text_draw(8*47, 14, (SDL_Color){ 0, 0, 0 }, coord);
    
    snprintf(coord, 3, "%u", cursor.w);
    text_draw(8*52, 14, (SDL_Color){ 0, 0, 0 }, coord);
    
    snprintf(coord, 3, "%u", cursor.h);
    text_draw(8*57, 14, (SDL_Color){ 0, 0, 0 }, coord);
  }
  
  if (current_mode == MODE_COPY && copy_show_copied) {
    text_draw(8*60, 14, (SDL_Color){ 0, 0, 0 }, "Copied");
  }

  for (unsigned x = 0; x < 80; x++) {
    for (unsigned y = 0; y < 24; y++) {
      SDL_Color override_bg_color;
      SDL_Color override_fg_color;
      
      SDL_Point cur_point = { x, y };
      
      if (current_mode != MODE_PREVIEW && current_mode != MODE_BG && current_mode != MODE_FG && current_mode != MODE_CHAR && SDL_PointInRect(&cur_point, &cursor)) {
        override_bg_color = ansi_color[fg_color[x][y]];
        override_fg_color = ansi_color[bg_color[x][y]];
      } else {
        override_bg_color = ansi_color[bg_color[x][y]];
        override_fg_color = ansi_color[fg_color[x][y]];
      }
      
      SDL_SetRenderDrawColor(renderer, override_bg_color.r, override_bg_color.g, override_bg_color.b, 255);
      SDL_Rect rect = { 8 + 8*x, 14*3 + 14*y, 8, 14 };
      SDL_RenderFillRect(renderer, &rect);
      
      text_draw_char(8 + 8*x, 14*3 + 14*y, override_fg_color, buffer[x][y]);
    }
  }
}
