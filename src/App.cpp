#include "App.hpp"
#include "UIManager.hpp"
#include "input.hpp"
#include "utils.hpp"
#include <SDL_events.h>
#include <SDL_render.h>

static SDL_Rect destRect{100, 100, TEXTURE_WIDTH, TEXTURE_HEIGHT};

App::~App() { UIManager::shutdown(); }

void App::init(_AppStartupFlags_ flags) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    return;
  }

  Uint32 window_flags = SDL_WINDOW_RESIZABLE;
  auto pWindow = SDL_CreateWindow("Bezier Playground", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                  WINDOW_HEIGHT, window_flags);
  if (!pWindow) {
    std::cerr << "SDL_Window error: " << SDL_GetError() << std::endl;
    return;
  }

  window_.reset(pWindow);

  auto pRenderer =
      SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (!pRenderer) {
    std::cerr << "SDL_Renderer error: " << SDL_GetError() << std::endl;
    return;
  }

  renderer_.reset(pRenderer);

  //   initialized_ = register_imgui(flags);

  backgroundColor_ = {
    .r = 0x0a,
    .g = 0xf0,
    .b = 0xff,
    .a = 0xff
  };

  initialized_ = UIManager::instance().init(*this);
}

void App::run() {
  while (appShouldRun_) {
    input();
    update();
    render();
  }
}

SDL_Window *App::getWindow() const { return window_.get(); }

SDL_Renderer *App::getRenderer() const { return renderer_.get(); }

void App::addTexture(SDL_Texture *texture) { textures_.push_back(texture); }

SDL_Texture *App::getTexture(size_t idx) const {
  if (idx < 0 || idx >= textures_.size())
    return nullptr;
  return textures_.at(idx);
}

void App::input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    bool uiEvent = UIManager::instance().input(&event);
    auto io = UIManager::instance().getIO();

    if (printState_) {
      print_state();
    }

    switch (event.type) {
    case SDL_QUIT:
      appShouldRun_ = false;
    case SDL_KEYDOWN:
      handleKeyboardEvent(event.key, appShouldRun_);
      return;
    case SDL_KEYUP:
      handleKeyboardEvent(event.key, appShouldRun_);
    case SDL_MOUSEBUTTONDOWN:
      if (!io.WantCaptureMouse) {
        handleMouseButtonEvent(event.button, circles_);
      }
      return;
    case SDL_MOUSEBUTTONUP:
      handleMouseButtonEvent(event.button, circles_);
      return;
    }
  }
}

void App::update() { return; }

void App::render() {

  //render ui
  UIManager::instance().render(*this);
  
  clear_background();

  // main rendering
  for (auto point : circles_) {
    render_circle(*this, point.x, point.y, 20);
  }
  
  present();
}

void App::present() const {
  SDL_SetRenderTarget(getRenderer(), nullptr);
  UIManager::instance().present();
  SDL_RenderPresent(getRenderer());
}

void App::clear_background() const {
  SDL_SetRenderDrawColor(getRenderer(), backgroundColor_.r, backgroundColor_.g, backgroundColor_.b, backgroundColor_.a);
  SDL_RenderClear(getRenderer()); // default target == window
}

void App::setBGColorF(float r, float g, float b) {
  backgroundColor_.r = r;
  backgroundColor_.g = g;
  backgroundColor_.b = b;
} 

void App::print_state() const {
  // utility to toggle some cout metrics for imgui state
  std::cout << "Imgui::GetIO(): \n";
  std::cout << "\tWantCaptureKeyboard: " << std::boolalpha
            << ImGui::GetIO().WantCaptureKeyboard << "\n";
  std::cout << "\tWantCaptureMouse: " << std::boolalpha
            << ImGui::GetIO().WantCaptureMouse << "\n";
  std::cout << "\tWantSetMousePos: " << std::boolalpha
            << ImGui::GetIO().WantSetMousePos;
  std::cout << std::endl;
}