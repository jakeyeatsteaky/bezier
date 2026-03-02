#include <SDL_pixels.h>
#include <SDL_render.h>
#include <iostream>

#include "App.hpp"
#include "utils.hpp"
#include "Texture.hpp"

// refactor for better architecture
// - [ ] Event registtration, so when I click, app listeners can update

// Things to add to app utilities window
// - [ ] color wheel to modify app background
// - [ ] buttons for dynamic ui options (features)
// - [ ] fps counter

int main(int, char **) {

  App app;

  // not subsystem flags, rather "feature" flags of things created within
  // imgui windows e.g. debug menu, various canvases
  constexpr uint32_t feature_window_flags = APP_STARTUP_FLAGS_BEZIER | APP_STARTUP_FLAGS_DEBUG;

  app.init(feature_window_flags); 

  // should Texture take a reference to the App or a pointer
  TextureConfig config = {
    .pixelFormat_ = SDL_PIXELFORMAT_RGBA8888,
    .access_ = SDL_TEXTUREACCESS_TARGET,
    .width_ = 660,
    .height_ = 440
  }; 

  Texture imguiTexture(std::move(config), app.getRenderer());

  app.addTexture(imguiTexture.getTexture());

  try {
    app.run();
  } catch (...) { 
    std::cerr << "An error has occurred." << std::endl;
  }

  return 0;
}


