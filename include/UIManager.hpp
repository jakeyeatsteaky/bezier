#pragma once

#include "UIComponent.hpp"
#include "imgui.h"

#include <vector>
#include <memory>

class UIComponent;
class App;
union SDL_Event;

using Components = std::vector<std::weak_ptr<UIComponent>>;

class UIManager {

    UIManager();
    static UIManager instance_;
public:
    ~UIManager() = default;
    static bool init(const App& app);
    static bool input(SDL_Event* event);
    static auto getIO() -> ImGuiIO&;
    static UIManager& instance();
    static void render(App& app); // atrocious...do not modify app in render loop. refactor this
    static void present();
    static void shutdown();
    void register_components();

    Components components_;
};