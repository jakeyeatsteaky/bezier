#include "input.hpp"
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <iostream>

void handleKeyboardEvent(SDL_KeyboardEvent key, bool& running)  {
    if (key.type == SDL_KEYDOWN) {
        handleKeydownEvent(key.keysym.sym, running);
    } else if (key.type == SDL_KEYUP) {
        handleKeyupEvent(key.keysym.sym);
    }
}

void handleKeydownEvent(SDL_Keycode sym, bool& running) {
    std::cout << "Key pressed: " << sym << std::endl;
    if (sym == SDLK_ESCAPE) {
        running = false;
    }
}

void handleKeyupEvent(SDL_Keycode sym) {
    std::cout << "Key release: " << sym << std::endl;
}

void handleMouseButtonEvent(SDL_MouseButtonEvent button, std::vector<SDL_Point>& circleVector) {
    if (button.type == SDL_MOUSEBUTTONDOWN) {
        std::cout << "Mouse Clicked at: " << button.x << ", " << button.y << std::endl;
        circleVector.push_back(SDL_Point(button.x, button.y));
    } else if ( button.type == SDL_MOUSEBUTTONUP) {
        std::cout << "Mouse Release at: " << button.x << ", " << button.y << std::endl;
    }
}