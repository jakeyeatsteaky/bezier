#include "UIManager.hpp"
#include "App.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
UIManager UIManager::instance_;
bool UIManager::renderDebug_ = true;
bool UIManager::renderBezier_ = true;

UIManager &UIManager::instance() { return instance_; }

UIManager::UIManager() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
}

bool UIManager::init(const App &app) {
  // Backends combo: SDL2 (platform backend)
  // SDL_Renderer (renderer backend)
  bool initBackend =
      ImGui_ImplSDL2_InitForSDLRenderer(app.getWindow(), app.getRenderer()) &
      ImGui_ImplSDLRenderer2_Init(app.getRenderer());

  if (!initBackend) {
    std::cerr << "Error initializing the ImGui backend" << std::endl;
    return false;
  }

  return true;
}

bool UIManager::input(SDL_Event *event) {
  return ImGui_ImplSDL2_ProcessEvent(event);
}

auto UIManager::getIO() -> ImGuiIO & { return ImGui::GetIO(); }

void UIManager::render(const App &app) {
  ImGui_ImplSDLRenderer2_NewFrame(); // order is renderer frame, platform
                                     // frame, imgui frame
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  // needs a key input to bring the menu back if
  // toggled off TODO
  if (renderDebug_) {
    render_debug_ui(app);
  }

  if (renderBezier_) {
    render_bezier_ui(app);
  }

}

void UIManager::present() {
  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::shutdown() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  std::cout << "Destroying ImGui Backends and Context" << std::endl;
}

// render ui utilities

void render_debug_ui(const App &app) {
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
  SDL_SetRenderTarget(app.getRenderer(), app.getTexture(0));
  SDL_SetRenderDrawColor(app.getRenderer(), 255, 0xAF, 255, 0xFF);
  SDL_RenderClear(app.getRenderer()); // flush the texture with the above color
  SDL_SetRenderTarget(app.getRenderer(), nullptr);
  ImGui::Begin("Bezier Canvas", nullptr, 0);
  ImGui::Image((ImTextureID)(intptr_t)app.getTexture(0),
               ImVec2((float)660.f, (float)440.f)); // dumb dumb, need to get wh from tesxture
  ImGui::End();
}

void render_bezier_ui(const App& app) {
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
  ImGui::Begin("App Utilities");
  ImGui::End();
}