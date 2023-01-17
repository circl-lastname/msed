#pragma once

#include <SDL.h>

void file_init();
void file_destroy();

void file_enter();
void file_leave();
void file_handle_key(SDL_Event* event);
void file_handle_input(SDL_Event* event);
void file_draw();
