#include <iostream>

#include "App.hpp"
#include "utils.hpp"
#include "Texture.hpp"

// TODO
// [ ] Put frame rate into Metrics window, add TTF_Font 
// [ ] Render circle via click

// refactor for better architecture
// - [ ] AppState
// - [ ] Event registtration, so when I click, app listeners can update
// - [ ] Lots of things to wire up, background should be dictated by an imgui color wheel,

// Refactor
// - [ ] maybe add imgui manager

int main(int, char **) {

  App app;

  // not subsystem flags, rather "feature" flags of things created within
  // imgui windows e.g. debug menu, various canvases
  constexpr uint32_t feature_window_flags = APP_STARTUP_FLAGS_BEZIER | APP_STARTUP_FLAGS_DEBUG;

  app.init(feature_window_flags); 

  // should Texture take a reference to the App or a pointer
  Texture imguiTexture(app.getRenderer());
  Texture texture(app.getRenderer());

  app.addTexture(imguiTexture.getTexture());
  app.addTexture(texture.getTexture());

  try {
    app.run();
  } catch (...) { 
    std::cerr << "An error has occurred." << std::endl;
  }

  return 0;
}


