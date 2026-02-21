#include "input.hpp"

void handleKeydownEvent(SDL_KeyboardEvent event, bool& running)  {
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: {
            running = false;
        }
        default: {

        }
    }

}