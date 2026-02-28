#include "App.hpp"
#include "input.hpp"
#include "utils.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

static SDL_Rect       destRect{100,100,TEXTURE_WIDTH, TEXTURE_HEIGHT};

App::~App() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

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
      SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_PRESENTVSYNC);
  if (!pRenderer) {
    std::cerr << "SDL_Renderer error: " << SDL_GetError() << std::endl;
    return;
  }

  renderer_.reset(pRenderer);

  initialized_ = register_imgui(flags);
}

bool App::register_imgui(_AppStartupFlags_ flags) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  // Backends combo: SDL2 (platform backend)
  // SDL_Renderer (renderer backend)
  bool initBackend = ImGui_ImplSDL2_InitForSDLRenderer(getWindow(), getRenderer()) &
         ImGui_ImplSDLRenderer2_Init(getRenderer());

    if (!initBackend) {
        std::cerr << "Error initializing the ImGui backend" << std::endl;
        return false;
    }

    // success == 0
    // therefore, after init 0x0100 would mean operation C failed
    //                     op: DCBA
    // right now it would just be 0x1001 since the only possible
    // flags are 0x1000 (APP_STARTUP_FLAGS_DEBUG) and 
    //           0x0001 (APP_STARTUP_FLAGS_BEZIER)

    int success = 0x0000;
    // handle init flags
    if (flags & APP_STARTUP_FLAGS_DEBUG) {
        success |= initialize_debug_window(*this); // return 0 on success 
    }

    if (flags & APP_STARTUP_FLAGS_BEZIER) {
        success |= initialize_bezier_window(*this);
    }

    if (success) {
        // TODO, create a decode_errors(success) function to get
        // which feature system failed from the encoded value
        std::cerr << "Error initializing some features: " << success << std::endl;
    }
    return success == 0; 
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
  auto io = ImGui::GetIO();
  while (SDL_PollEvent(&event)) {

    ImGui_ImplSDL2_ProcessEvent(&event);

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
  render_imgui();

  const double now = (double)SDL_GetTicks() / 1000; // seconds
  const float red = 0.5 + 0.5 * SDL_sin(now);
  const float green = 0.5 + 0.5 * SDL_sin(now + PI * 1 / 3);
  const float blue = 0.5 + 0.5 * SDL_sin(now + PI * 5 / 3);

  const uint8_t r = static_cast<uint8_t>(red * 0xFF);
  const uint8_t g = static_cast<uint8_t>(green * 0xFF);
  const uint8_t b = static_cast<uint8_t>(blue * 0xFF);

  SDL_SetRenderDrawColor(getRenderer(), r, g, b, 0xFF);
  SDL_RenderClear(getRenderer()); // default target == window

  for (auto point : circles_) {
    render_circle(*this, point.x, point.y, 20);
  }
  // Rendering happens here
  SDL_SetRenderDrawColor(getRenderer(), g, r, b, 0xFF);
  SDL_SetRenderTarget(getRenderer(), getTexture(1));
  SDL_RenderClear(getRenderer());
  SDL_SetRenderDrawColor(getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderDrawLine(getRenderer(), 0, 0, 100, 100);
  SDL_SetRenderTarget(getRenderer(), nullptr);
  SDL_RenderCopy(getRenderer(), getTexture(1), NULL, &destRect);
  present();
}

void App::render_imgui() {
  ImGui_ImplSDLRenderer2_NewFrame(); // order is renderer frame, platform
                                     // frame, imgui frame
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  ImDrawList* dl = ImGui::GetWindowDrawList();
  ImGuiIO &io = ImGui::GetIO();
//   ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
//   ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
//   SDL_SetRenderTarget(getRenderer(), getTexture(0));
//   SDL_SetRenderDrawColor(getRenderer(), 255, 0xAF, 255, 0xFF);
//   SDL_RenderClear(getRenderer());// flush the texture with the above color
//   SDL_SetRenderTarget(getRenderer(), nullptr);
//   ImGui::Begin("Metrics", nullptr, 0);
//   ImGui::Image((ImTextureID)(intptr_t)getTexture(0),
//                ImVec2((float)TEXTURE_WIDTH, (float)TEXTURE_HEIGHT));
//   ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(200,200), ImGuiCond_FirstUseEver);
  ImGui::Begin("App Utilities");
  bool buttonClicked = ImGui::Button("Toggle App Util");
  printState_ = (buttonClicked) ? !printState_ : printState_;
  ImGui::End();


  SDL_RenderSetScale(getRenderer(), io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
}

void App::present() const {
  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(getRenderer());
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