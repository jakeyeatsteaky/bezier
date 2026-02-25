#pragma once
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <vector>

void handleKeyboardEvent(SDL_KeyboardEvent event, bool& running);
void handleKeydownEvent(SDL_Keycode sym, bool& runnning);
void handleKeyupEvent(SDL_Keycode sym);
void handleMouseButtonEvent(SDL_MouseButtonEvent event, std::vector<SDL_Point>& circles);