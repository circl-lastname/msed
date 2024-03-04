#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "error.h"
#include "text.h"
#include "titlebar.h"
#include "mode.h"

#include "file.h"
#include "editor.h"

SDL_Window* window;
SDL_Renderer* renderer;

static bool will_draw = false;

void main_quit() {
  text_destroy();
  file_destroy();
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}

void main_draw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
  SDL_RenderClear(renderer);
  
  titlebar_draw();
  mode_draw();
  
  SDL_RenderPresent(renderer);
}

void main_will_draw() {
  will_draw = true;
}

int main() {
  HE(SDL_Init(SDL_INIT_VIDEO) < 0);
  
  HE(SDL_CreateWindowAndRenderer(656, 392, SDL_WINDOW_BORDERLESS, &window, &renderer) < 0);
  
  // Prevent dubious duplicated key press at start.
  SDL_StopTextInput();
  
  text_init();
  titlebar_init();
  file_init();
  editor_init();
  
  main_draw();
  
  while (true) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    
    while (true) {
      switch (event.type) {
        case SDL_QUIT:
          main_quit();
        break;
        case SDL_KEYDOWN:
          mode_handle_key(&event);
        break;
        case SDL_TEXTINPUT:
          mode_handle_input(&event);
        break;
        case SDL_MOUSEBUTTONUP:
          if (event.button.button == SDL_BUTTON_LEFT) {
            titlebar_handle_click(&event);
          }
        break;
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
            main_will_draw();
          }
        break;
      }
      
      if (!SDL_PollEvent(&event)) {
        break;
      }
    }
    
    if (will_draw) {
      main_draw();
      will_draw = false;
    }
  }
 return 0;
}
