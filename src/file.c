#include <errno.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "dir.h"
#include "main.h"
#include "text.h"
#include "titlebar.h"
#include "textbox.h"
#include "editor.h"

static char title[7+81] = "MSED - ";

static entries_t* entries = NULL;

static char* message = NULL;
static bool message_error;

static textbox_t* textbox;

static unsigned option_view = 0;
static unsigned option = 0;

static unsigned button = 0;

static unsigned area = 0;

static void handle_change() {
  if (entries) {
    free(entries);
  }
  
  entries = dir_read(textbox->text);
  
  message = NULL;
  
  option_view = 0;
  option = 0;
}

void file_init() {
  HE((textbox = malloc(sizeof(textbox_t) + 81)) == NULL);
  
  *textbox = (textbox_t){
    .x = 8,
    .y = 14*3,
    .cursor = 0,
    .cursor_show = true,
    .handle_change = handle_change,
    .capacity = 81,
    .length = 0,
  };
  
  if (getcwd(textbox->text, 81)) {
    textbox->length = strlen(textbox->text);
    textbox->cursor = textbox->length;
  } else {
    textbox->text[0] = '\0';
    textbox->length = 0;
    textbox->cursor = 0;
  }
  
  handle_change();
}

void file_destroy() {
  free(textbox);
  free(entries);
}

void file_enter() {
  if (area == 0) {
    SDL_StartTextInput();
  }
}

void file_leave() {
  if (area == 0) {
    SDL_StopTextInput();
  }
}

static void set_view() {
  if (entries->amount > 20) {
    if (option <= 10) {
      option_view = 0;
    } else if (option > entries->amount - 10) {
      option_view = entries->amount - 20;
    } else {
      option_view = option - 10;
    }
  }
}

void file_handle_key(SDL_Event* event) {
  if (event->key.keysym.sym == SDLK_TAB) {
    switch (area) {
      case 0:
        SDL_StopTextInput();
        textbox->cursor_show = false;
      break;
      case 2:
        SDL_StartTextInput();
        textbox->cursor_show = true;
      break;
    }
    
    area = (area + 1) % 3;
    
    return;
  }
  
  switch (area) {
    case 0:
      textbox_handle_key(textbox, event);
    break;
    case 1:
      switch (event->key.keysym.sym) {
        case SDLK_UP:
          if (option != 0) {
            option--;
            
            set_view();
          }
        break;
        case SDLK_DOWN:
          if (option != entries->amount - 1) {
            option++;
            
            set_view();
          }
        break;
        case SDLK_PAGEUP:
          if ((int)option - 10 >= 0) {
            option -= 10;
          } else {
            option = 0;
          }
          
          set_view();
        break;
        case SDLK_PAGEDOWN:
          if (option + 10 <= entries->amount - 1) {
            option += 10;
          } else {
            option = entries->amount - 1;
          }
          
          set_view();
        break;
        case SDLK_HOME:
          option = 0;
          
          set_view();
        break;
        case SDLK_END:
          option = entries->amount - 1;
          
          set_view();
        break;
        case SDLK_RETURN:
          if (entries->entries[option].type != ENTRY_TYPE_ERROR) {
            char* name;
            HE((name = malloc(strlen(textbox->text) + 1 + strlen(entries->entries[option].name) + 1)) == NULL);
            
            strcpy(name, textbox->text);
            strcat(name, "/");
            strcat(name, entries->entries[option].name);
            
            char* real = realpath(name, NULL);
            free(name);
            
            if (real) {
              size_t real_length = strlen(real);
              
              if (real_length < 81) {
                strcpy(textbox->text, real);
                
                textbox->length = real_length;
                textbox->cursor = textbox->length;
                
                handle_change();
              }
              
              free(real);
            }
          }
        break;
      }
    break;
    case 2:
      switch (event->key.keysym.sym) {
        case SDLK_LEFT:
        case SDLK_RIGHT:
          button = (button + 1) % 2;
        break;
        case SDLK_RETURN:
          FILE* file = NULL;
          
          switch (button) {
            case 0:
              file = fopen(textbox->text, "wb");
            break;
            case 1:
              file = fopen(textbox->text, "rb+");
            break;
          }
          
          if (!file) {
            message = strerror(errno);
            message_error = true;
            
            return;
          }
          
          switch (button) {
            case 0:
              editor_save(file);
              message = "File saved successfully";
            break;
            case 1:
              editor_load(file);
              message = "File loaded successfully";
            break;
          }
          
          message_error = false;
          
          fclose(file);
          
          strcpy(&title[7], textbox->text);
          titlebar_set(title);
        break;
      }
    break;
  }
}

void file_handle_input(SDL_Event* event) {
  textbox_handle_input(textbox, event);
}

void file_draw() {
  SDL_Rect rect;
  
  if (message) {
    SDL_Color color;
    
    if (message_error) {
      color = (SDL_Color){ 255, 0, 0 };
    } else {
      color = (SDL_Color){ 0, 0, 0 };
    }
    
    text_draw(8*40, 14, color, message);
  }
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  rect = (SDL_Rect){ 8, 14*3, 8*80, 14 };
  SDL_RenderFillRect(renderer, &rect);
  
  textbox_draw(textbox);
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  rect = (SDL_Rect){ 8, 14*5, 8*80, 14*20 };
  SDL_RenderFillRect(renderer, &rect);
  
  if (area == 1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    rect = (SDL_Rect){ 8, 14*5 + 14*(option - option_view), 8*80, 14 };
    SDL_RenderFillRect(renderer, &rect);
  }
  
  for (unsigned i = 0; i < 20 && i + option_view < entries->amount; i++) {
    SDL_Color color;
    
    switch (entries->entries[i + option_view].type) {
      case ENTRY_TYPE_FILE:
        color = (SDL_Color){ 255, 255, 255 };
      break;
      case ENTRY_TYPE_DIR:
        color = (SDL_Color){ 255, 255, 0 };
      break;
      case ENTRY_TYPE_ERROR:
        color = (SDL_Color){ 255, 0, 0 };
      break;
    }
    
    text_draw(8, 14*5 + 14*i, color, entries->entries[i + option_view].name);
  }
  
  if (area == 2) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    
    switch (button) {
      case 0:
        rect = (SDL_Rect){ 8, 14*26, 8*21, 14 };
      break;
      case 1:
        rect = (SDL_Rect){ 8*23, 14*26, 8*11, 14 };
      break;
    }
    
    SDL_RenderFillRect(renderer, &rect);
  }
  
  text_draw(8, 14*26, (SDL_Color){ 255, 255, 255 }, "[Create or Save file] [Open file]");
}
