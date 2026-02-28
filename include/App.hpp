#pragma once 
#include "utils.hpp"
#include <SDL.h>
#include <SDL_quit.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <memory>
#include <iostream>
#include <vector>
#include <utility>

namespace AppDeleter {
    struct WindowDeleter {
        void operator()(SDL_Window* window) {
            if (window) {
                SDL_DestroyWindow(window);
                std::cout << "Deleting SDL Window" << std::endl;
                SDL_Quit();
                std::cout << "Deleting SDL Application" << std::endl;


            }
        }
    };

    struct RendererDeleter {
        void operator()(SDL_Renderer* renderer) {
            if (renderer) {
                SDL_DestroyRenderer(renderer);
                std::cout << "Deleting SDL Renderer" << std::endl;
            }
        }
    };
}

class App {
    typedef int SDL_Context;
    App(App&&) = delete;
    App(const App&) = delete;
    App& operator=(App&&) = delete;
    App& operator=(const App&) = delete;

    bool register_imgui(_AppStartupFlags_ flags);
    void render_imgui();
    void present() const;
    void print_state()const;
    
    void input();
    void update();
    void render();

    std::unique_ptr<SDL_Window, AppDeleter::WindowDeleter> window_;
    std::unique_ptr<SDL_Renderer, AppDeleter::RendererDeleter> renderer_;
    std::vector<SDL_Texture*> textures_;
    bool appShouldRun_ = true;
    bool printState_ = false;
public:
    bool initialized_ = false;

    App() = default;
    ~App();
    void init(_AppStartupFlags_ flags);
    void run();

   void addTexture(SDL_Texture* texture);
   SDL_Texture* getTexture(size_t idx) const; 

   std::vector<SDL_Point> circles_;

    SDL_Window* getWindow() const;
    SDL_Renderer* getRenderer() const;

};