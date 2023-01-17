#include <SDL.h>
#include <string.h>

#include "main.h"
#include "text.h"
#include "textbox.h"

void textbox_handle_key(textbox_t* textbox, SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_LEFT:
      if (textbox->cursor != 0) {
        textbox->cursor--;
      }
    break;
    case SDLK_RIGHT:
      if (textbox->cursor != textbox->length) {
        textbox->cursor++;
      }
    break;
    case SDLK_HOME:
      textbox->cursor = 0;
    break;
    case SDLK_END:
      textbox->cursor = textbox->length;
    break;
    case SDLK_BACKSPACE:
      if (textbox->cursor != 0) {
        if (textbox->cursor == textbox->length) {
          textbox->text[textbox->cursor - 1] = '\0';
        } else {
          memmove(textbox->text + textbox->cursor - 1, textbox->text + textbox->cursor, textbox->length + 1 - textbox->cursor);
        }
        
        textbox->cursor--;
        textbox->length--;
        
        textbox->handle_change();
      }
    break;
    case SDLK_DELETE:
      if (textbox->cursor != textbox->length) {
        if (textbox->cursor == textbox->length - 1) {
          textbox->text[textbox->cursor] = '\0';
        } else {
          memmove(textbox->text + textbox->cursor, textbox->text + textbox->cursor + 1, textbox->length + 1 - textbox->cursor - 1);
        }
        
        textbox->length--;
        
        textbox->handle_change();
      }
    break;
    case SDLK_v:
      if (event->key.keysym.mod & KMOD_LCTRL || event->key.keysym.mod & KMOD_RCTRL) {
        char* clipboard = SDL_GetClipboardText();
        size_t length = strlen(clipboard);
        
        if (textbox->length + length < textbox->capacity - 1) {
          if (textbox->cursor != textbox->length) {
            memmove(textbox->text + textbox->cursor + length, textbox->text + textbox->cursor, textbox->length + 1 - textbox->cursor);
            memcpy(textbox->text + textbox->cursor, clipboard, length);
          } else {
            strcat(textbox->text, clipboard);
          }
          
          textbox->cursor += length;
          textbox->length += length;
          
          textbox->handle_change();
        }
        
        SDL_free(clipboard);
      }
    break;
  }
}

void textbox_handle_input(textbox_t* textbox, SDL_Event* event) {
  if (textbox->length + 1 < textbox->capacity - 1) {
    if (textbox->cursor == textbox->length) {
      textbox->text[textbox->cursor] = event->text.text[0];
      textbox->text[textbox->cursor + 1] = '\0';
    } else {
      memmove(textbox->text + textbox->cursor + 1, textbox->text + textbox->cursor, textbox->length + 1 - textbox->cursor);
      textbox->text[textbox->cursor] = event->text.text[0];
    }
    
    textbox->cursor++;
    textbox->length++;
    
    textbox->handle_change();
  }
}

void textbox_draw(textbox_t* textbox) {
  if (textbox->cursor_show) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect = { textbox->x + 8*textbox->cursor, textbox->y, 8, 14 };
    SDL_RenderFillRect(renderer, &rect);
  }
  
  text_draw(textbox->x, textbox->y, (SDL_Color){ 255, 255, 255 }, textbox->text);
}
