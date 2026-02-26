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

#include <SDL_video.h>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "App.hpp"
#include "Texture.hpp"
#include "utils.hpp"
#include "input.hpp"

static SDL_Texture*   gTexture = nullptr;
static SDL_Texture*   gTexture2 = nullptr;
static SDL_Rect       destRect{100,100,TEXTURE_WIDTH, TEXTURE_HEIGHT};
static bool gRunning = true;

std::vector<SDL_Point> gCircles{};

// TODO
// [ ] Put frame rate into Metrics window, add TTF_Font 
// [ ] Render circle via click

// refactor for better architecture
// - [ ] AppState
// - [ ] Event registtration, so when I click, app listeners can update
// - [ ] Lots of things to wire up, background should be dictated by an imgui color wheel,

bool init(const App& app);
bool init_imgui(const App& app);
void input();
void update();
void render(const App& app);
void render_imgui(const App& app);
void present(const App& app);
void cleanup();

void render_circle(const App& app, int x, int y, int radius);

int main(int, char **) {

  App app;

  app.init();

  // should Texture take a reference to the App or a pointer
  Texture imguiTexture(app.getRenderer());
  Texture texture(app.getRenderer());

  app.addTexture(imguiTexture.getTexture());
  app.addTexture(texture.getTexture());

  if (!init(app)) {
    return -1;
  }
  
  while (gRunning) {
    input();
    update();
    render(app);
  }

  cleanup();
  return 0;
}

bool init(const App& app) {

  return init_imgui(app);
}

bool init_imgui(const App& app) {

  auto gWindow = app.getWindow();
  auto gRenderer = app.getRenderer();
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
    switch (event.type) {
      case SDL_QUIT:
        gRunning = false;
      case SDL_KEYDOWN: 
        handleKeyboardEvent(event.key, gRunning);
        return;
      case SDL_KEYUP:
        handleKeyboardEvent(event.key, gRunning);
      case SDL_MOUSEBUTTONDOWN:
        handleMouseButtonEvent(event.button, gCircles);
        return;
      case SDL_MOUSEBUTTONUP:
        handleMouseButtonEvent(event.button, gCircles);
        return;

    }

  }
}

void update() {
  return;
}

void render_imgui(const App& app) {
  auto gRenderer = app.getRenderer();
  ImGui_ImplSDLRenderer2_NewFrame(); // order is renderer frame, platform
                                     // frame, imgui frame
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
  // ImDrawList* dl = ImGui::GetWindowDrawList();
  ImGuiIO &io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_FirstUseEver);
  SDL_SetRenderTarget(gRenderer, app.getTexture(0));
  SDL_SetRenderDrawColor(gRenderer, 255, 0xAF, 255, 0xFF);
  SDL_RenderClear(gRenderer);// flush the texture with the above color
  SDL_SetRenderTarget(gRenderer, nullptr);
  ImGui::Begin("Metrics", nullptr, 0);
  ImGui::Image((ImTextureID)(intptr_t)app.getTexture(0),
               ImVec2((float)TEXTURE_WIDTH, (float)TEXTURE_HEIGHT));
  ImGui::End();

  SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
}

void present(const App& app) {
  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(app.getRenderer());
}

void render(const App& app) {
  auto gRenderer = app.getRenderer(); 
  render_imgui(app);

  const double now = (double)SDL_GetTicks() / 1000; // seconds
  const float red = 0.5 + 0.5 * SDL_sin(now);
  const float green = 0.5 + 0.5 * SDL_sin(now + PI * 1 / 3);
  const float blue = 0.5 + 0.5 * SDL_sin(now + PI * 5 / 3);
  
  const uint8_t r = static_cast<uint8_t>(red * 0xFF);
  const uint8_t g = static_cast<uint8_t>(green * 0xFF);
  const uint8_t b = static_cast<uint8_t>(blue * 0xFF);
  
  SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);
  SDL_RenderClear(gRenderer); // default target == window
  
  for (auto point : gCircles) {
    render_circle(app, point.x, point.y, 20);
  }
  // Rendering happens here
  SDL_SetRenderDrawColor(gRenderer, g, r, b, 0xFF);
  SDL_SetRenderTarget(gRenderer, app.getTexture(1));
  SDL_RenderClear(gRenderer);
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderDrawLine(gRenderer, 0, 0, 100, 100);
  SDL_SetRenderTarget(gRenderer, nullptr);
  SDL_RenderCopy(gRenderer, app.getTexture(1), NULL, &destRect);
  present(app);
}

void cleanup() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void render_circle(const App& app, int x, int y, int r) {
  Uint8 red = 0xFF;
  Uint8 green = 0x0F;
  Uint8 blue = 0x5F;
  SDL_SetRenderDrawColor(app.getRenderer(), red, green, blue, 0xFF);

  std::vector<SDL_Point> pointBuffer{};
  
  //bounds
  int upperBound = y + r;
  int lowerBound = y - r;

  // TODO -- improve using symmetry
  for (int i = lowerBound; i < upperBound; i++) {
    int xLeft = x - sqrt(r*r - (std::pow(i - y,2)));
    int xRight = x + sqrt(r*r - (std::pow(i - y,2)));

    while (xLeft < xRight) {
      int xValue = xLeft;
      pointBuffer.push_back(SDL_Point{xValue,i});
      xLeft++;
    }
  }

  SDL_RenderDrawPoints(app.getRenderer(), pointBuffer.data(), pointBuffer.size());
}