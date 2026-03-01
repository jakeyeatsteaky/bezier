#include "UIManager.hpp"
#include "App.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
UIManager UIManager::instance_;

UIManager& UIManager::instance() { return instance_; }

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

  // success == 0
  // therefore, after init 0x0100 would mean operation C failed
  //                     op: DCBA
  // right now it would just be 0x1001 since the only possible
  // flags are 0x1000 (APP_STARTUP_FLAGS_DEBUG) and
  //           0x0001 (APP_STARTUP_FLAGS_BEZIER)

  // int success = 0x0000;
  // // handle init flags
  // if (flags & APP_STARTUP_FLAGS_DEBUG) {
  //     success |= initialize_debug_window(*this); // return 0 on success
  // }

  // if (flags & APP_STARTUP_FLAGS_BEZIER) {
  //     success |= initialize_bezier_window(*this);
  // }

  // if (success) {
  //     // TODO, create a decode_errors(success) function to get
  //     // which feature system failed from the encoded value
  //     std::cerr << "Error initializing some features: " << success <<
  //     std::endl;
  //}

  // right here intitalize this ImGuI manager class with
  // RAII to handle begin/end conditional logic.
  // .....maybe app flags can be tied to the bool members of this
  //  initially?

  // return success == 0;
  return true;
}


bool UIManager::input(SDL_Event *event) {
  return ImGui_ImplSDL2_ProcessEvent(event);
}

auto UIManager::getIO() -> ImGuiIO & { return ImGui::GetIO(); }

void UIManager::render(App &app) {
  ImGui_ImplSDLRenderer2_NewFrame(); // order is renderer frame, platform
                                     // frame, imgui frame
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
  SDL_SetRenderTarget(app.getRenderer(), app.getTexture(0));
  SDL_SetRenderDrawColor(app.getRenderer(), 255, 0xAF, 255, 0xFF);
  SDL_RenderClear(app.getRenderer()); // flush the texture with the above color
  SDL_SetRenderTarget(app.getRenderer(), nullptr);
  ImGui::Begin("Metrics", nullptr, 0);
  ImGui::Image((ImTextureID)(intptr_t)app.getTexture(0),
               ImVec2((float)TEXTURE_WIDTH, (float)TEXTURE_HEIGHT));
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
  ImGui::Begin("App Utilities");
  bool buttonClicked = ImGui::Button("Toggle App Util");
  app.printState_ = (buttonClicked) ? !app.printState_ : app.printState_;
  ImGui::End();

  SDL_RenderSetScale(app.getRenderer(), io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
}

void UIManager::present() {
  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::shutdown() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}