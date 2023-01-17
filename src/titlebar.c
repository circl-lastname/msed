#include <SDL.h>

#include "main.h"
#include "text.h"
#include "titlebar.h"

static char* current_title;

SDL_HitTestResult hit_test(SDL_Window* unused_window, const SDL_Point* point, void* unused_data) {
  if (point->x < 656 - 8*7 && point->y < 14) {
    return SDL_HITTEST_DRAGGABLE;
  } else {
    return SDL_HITTEST_NORMAL;
  }
}

void titlebar_init() {
  SDL_SetWindowHitTest(window, hit_test, NULL);
  titlebar_set("MSED");
}

void titlebar_set(char* title) {
  current_title = title;
  SDL_SetWindowTitle(window, current_title);
}

void titlebar_handle_click(SDL_Event* event) {
  SDL_Point point = { event->button.x, event->button.y };
  
  SDL_Rect minimize = { 656 - 8*7, 0, 8*3, 14 };
  SDL_Rect close = { 656 - 8*3, 0, 8*3, 14 };
  
  if (SDL_PointInRect(&point, &minimize)) {
    SDL_MinimizeWindow(window);
  } else if (SDL_PointInRect(&point, &close)) {
    main_quit();
  }
}

void titlebar_draw() {
  SDL_Rect rect;
  
  SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
  rect = (SDL_Rect){ 0, 0, 656, 14 };
  SDL_RenderFillRect(renderer, &rect);
  
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  rect = (SDL_Rect){ 656 - 8*7, 0, 8*3, 14 };
  SDL_RenderFillRect(renderer, &rect);
  
  text_draw(8, 0, (SDL_Color){ 255, 255, 255 }, current_title);
  
  rect = (SDL_Rect){ 656 - 8*3, 0, 8*3, 14 };
  SDL_RenderFillRect(renderer, &rect);
  
  text_draw(656 - 8*7, 0, (SDL_Color){ 255, 255, 255 }, "[_] [X]");
}
