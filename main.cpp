#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>

#include <array>

constexpr int WIDTH = 100;
constexpr int HEIGHT = 100;

std::array<uint32_t, WIDTH*HEIGHT> pixels_;

int main(int, char**) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
  SDL_Window* window = SDL_CreateWindow("Bezier Playground",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_RESIZABLE);
  if (!window) return 1;
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
    SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) return 1;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer); // Backends combo: SDL2 (platform backend)
  ImGui_ImplSDLRenderer2_Init(renderer);               //                 SDL_Renderer (renderer backend)

  // main loop:
    // Poll events
    // Update application/game state
    // Render contents into a framebuffer
    // Swap/Present framebuffer to the screen
    // Wait some time (e.g. 1/60 of a second)
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) running = false;
    }

    ImGui_ImplSDLRenderer2_NewFrame(); // order is renderer frame, platform frame, imgui frame
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
    
    // frame buffer
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT), ImGuiCond_FirstUseEver);
    ImGui::Begin("NextWindow");
    
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(560, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(464, 768), ImGuiCond_FirstUseEver);
    ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoScrollbar);
    ImVec2 canvasPos = ImGui::GetWindowPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    ImVec2 canvasMin = ImGui::GetWindowContentRegionMin();
    ImVec2 canvasScreenMin = ImVec2(canvasPos.x + canvasMin.x, canvasPos.y + canvasMin.y);
    ImGui::End();
    
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddCircle(ImVec2(50,50), 4.0, 0xff00ffff);

    //dl->AddCallback(callback, pixels_.data())

    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
    SDL_RenderClear(renderer);

    // Other rendering for my application comes here


    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
  }

  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
