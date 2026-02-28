#include "App.hpp"
#include "utils.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

void App::init() {
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
  

  auto pRenderer = SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_PRESENTVSYNC);
  if (!pRenderer) {
    std::cerr << "SDL_Renderer error: " << SDL_GetError() << std::endl;
    return;
  }

  renderer_.reset(pRenderer);

  initialized_ = register_imgui();
}

bool App::register_imgui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  // Backends combo: SDL2 (platform backend)
  // SDL_Renderer (renderer backend)
  return ImGui_ImplSDL2_InitForSDLRenderer(getWindow(), getRenderer()) &
            ImGui_ImplSDLRenderer2_Init(getRenderer());
}

SDL_Window* App::getWindow() const {
    return window_.get();
}

SDL_Renderer* App::getRenderer() const {
    return renderer_.get();
}

void App::addTexture(SDL_Texture* texture) {
    textures_.push_back(texture);
}

SDL_Texture* App::getTexture(size_t idx) const {
    if (idx < 0 || idx >= textures_.size()) return nullptr;
    return textures_.at(idx);
}