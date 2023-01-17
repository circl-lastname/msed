#pragma once

#include <SDL.h>

void titlebar_init();

void titlebar_set(char* title);
void titlebar_handle_click(SDL_Event* event);
void titlebar_draw();
