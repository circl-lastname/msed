#pragma once

#include <SDL.h>
#include <stdio.h>

void editor_init();

void editor_save(FILE* file);
void editor_load(FILE* file);

void editor_select_handle_key(SDL_Event* event);
void editor_bg_enter();
void editor_bg_handle_key(SDL_Event* event);
void editor_fg_enter();
void editor_fg_handle_key(SDL_Event* event);
void editor_char_enter();
void editor_char_handle_key(SDL_Event* event);
void editor_copy_enter();
void editor_copy_handle_key(SDL_Event* event);
void editor_paste_enter();
void editor_paste_handle_key(SDL_Event* event);
void editor_text_enter();
void editor_text_leave();
void editor_text_handle_key(SDL_Event* event);
void editor_text_handle_input(SDL_Event* event);
void editor_draw();
