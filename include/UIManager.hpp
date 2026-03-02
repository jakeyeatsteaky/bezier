#pragma once

#include "UIComponent.hpp"
#include "imgui.h"

#include <vector>
#include <memory>

class UIComponent;
class App;
union SDL_Event;

using Components = std::vector<std::weak_ptr<UIComponent>>;

void render_debug_ui(const App&);
void render_bezier_ui(const App&);
class UIManager {

    UIManager();
    static UIManager instance_;
    static bool renderDebug_;
    static bool renderBezier_;
public:
    ~UIManager() = default;
    static bool init(const App& app);
    static bool input(SDL_Event* event);
    static auto getIO() -> ImGuiIO&;
    static UIManager& instance();
    static void render(const App& app); 
    static void present();
    static void shutdown();
    void register_components();

    Components components_;
};