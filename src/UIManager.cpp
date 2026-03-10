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

//TODO move color picker background to update and make this app const
void UIManager::render(App &app) {
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
static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
void render_bezier_ui(App &app) {
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
  ImGui::Begin("Bezier Canvas", nullptr, 0);
  ImGui::ColorPicker3("Background Color", (float*)&color);
  app.setBGColorF(color.x, color.y, color.z);
  ImGui::End();
}

void render_debug_ui(const App& app) {
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
  ImGui::Begin("App Utilities");
  ImGui::End();
}