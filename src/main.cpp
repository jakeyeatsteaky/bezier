#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>

#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "input.hpp"

static SDL_Window *gWindow = nullptr;
static SDL_Renderer *gRenderer = nullptr;
static SDL_Texture *gTexture = nullptr;
static bool gRunning = true;

bool init();
bool init_imgui();
void input();
void update();
void render();
void render_imgui();
void present();
void cleanup();

Start here:
add to the event callbacks
get mouse click to add rects 
bezier between them

int main(int, char **) {

  if (!init()) {
    return -1;
  }

  while (gRunning) {
    input();
    update();
    render();
  }

  cleanup();
  return 0;
}

bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    return false;
  }

  gWindow = SDL_CreateWindow("Bezier Playground", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                             WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!gWindow) {
    std::cerr << "SDL_Window error: " << SDL_GetError() << std::endl;
    return false;
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
  if (!gRenderer) {
    std::cerr << "SDL_Renderer error: " << SDL_GetError() << std::endl;
    return false;
  }

  gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888,
                               SDL_TEXTUREACCESS_TARGET, TEXTURE_WIDTH,
                               TEXTURE_HEIGHT);

  if (!gTexture) {
    std::cerr << "SDL_Texture error: " << SDL_GetError() << std::endl;
    return false;
  }

  return init_imgui();
}

bool init_imgui() {

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  bool success1 = ImGui_ImplSDL2_InitForSDLRenderer(
      gWindow, gRenderer); // Backends combo: SDL2 (platform backend)
  bool success2 = ImGui_ImplSDLRenderer2_Init(gRenderer); // SDL_Renderer (renderer backend)

  return success2&success1;
}

void input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
      gRunning = false;

    if(event.type == SDL_KEYDOWN) {
      handleKeydownEvent(event.key, gRunning);
    }
  }
}

void update() {
  return;
}

void render_imgui() {
  ImGui_ImplSDLRenderer2_NewFrame(); // order is renderer frame, platform
                                     // frame, imgui frame
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  // ImDrawList* dl = ImGui::GetWindowDrawList();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
  SDL_SetRenderTarget(gRenderer, gTexture);
  SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
  SDL_RenderClear(gRenderer);
  SDL_SetRenderTarget(gRenderer, nullptr);
  ImGui::Begin("TestWindow", nullptr, 0);
  ImGui::Image((ImTextureID)(intptr_t)gTexture,
               ImVec2((float)TEXTURE_WIDTH, (float)TEXTURE_HEIGHT));
  ImGui::End();

  SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
}

void present() {
  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(gRenderer);
}

void render() {
  render_imgui();

  const double now = (double)SDL_GetTicks() / 1000; // seconds
  const float red = 0.5 + 0.5 * SDL_sin(now);
  const float green = 0.5 + 0.5 * SDL_sin(now + PI * 1 / 3);
  const float blue = 0.5 + 0.5 * SDL_sin(now + PI * 5 / 3);

  const uint8_t r = static_cast<uint8_t>(red * 0xFF);
  const uint8_t g = static_cast<uint8_t>(green * 0xFF);
  const uint8_t b = static_cast<uint8_t>(blue * 0xFF);

  SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);
  SDL_RenderClear(gRenderer);

  // Other rendering for my application comes here

  // SDL_SetRenderDrawColor(gRenderer, g, r, b, 0xFF);

  present();
}

void cleanup() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
}