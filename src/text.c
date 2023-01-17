#include <SDL.h>
#include <string.h>

#include "main.h"
#include "font.h"

static SDL_Texture* font;

void text_init() {
  // Weird behavior, at depth 1, SDL ignores the mask and reverses colors. This is for now overcome by reversing the font before conversion and setting a color key.
  SDL_Surface* font_surface = SDL_CreateRGBSurfaceFrom(font_pixels, 256, 112, 1, 32, 0, 0, 0, 0);
  SDL_SetColorKey(font_surface, SDL_TRUE, SDL_MapRGB(font_surface->format, 0, 0, 0));
  
  font = SDL_CreateTextureFromSurface(renderer, font_surface);
  
  SDL_FreeSurface(font_surface);
}

void text_destroy() {
  SDL_DestroyTexture(font);
}

void text_draw(unsigned x, unsigned y, SDL_Color color, unsigned char* text) {
  unsigned cur_x = x;
  unsigned cur_y = y;
  
  SDL_SetTextureColorMod(font, color.r, color.g, color.b);
  
  for (size_t i = 0; text[i] != '\0'; i++) {
    if (text[i] == '\n') {
      cur_x = x;
      cur_y += 14;
      continue;
    }
    
    SDL_Rect ch = { text[i] % 32 * 8, text[i] / 32 * 14, 8, 14 };
    SDL_Rect dest = { cur_x, cur_y, 8, 14 };
    SDL_RenderCopy(renderer, font, &ch, &dest);
    
    cur_x += 8;
  }
}

void text_draw_char(unsigned x, unsigned y, SDL_Color color, unsigned char ch) {
  SDL_SetTextureColorMod(font, color.r, color.g, color.b);
  
  SDL_Rect ch_rect = { ch % 32 * 8, ch / 32 * 14, 8, 14 };
  SDL_Rect dest = { x, y, 8, 14 };
  SDL_RenderCopy(renderer, font, &ch_rect, &dest);
}
