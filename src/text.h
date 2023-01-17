#pragma once

#include <SDL.h>
#include <stddef.h>

void text_init();
void text_destroy();

void text_draw(unsigned x, unsigned y, SDL_Color color, unsigned char* text);
void text_draw_char(unsigned x, unsigned y, SDL_Color color, unsigned char ch);
